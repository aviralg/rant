annotations <- function(object, type) {
  UseMethod("annotations", object)
}

annotations.default <- function(object, ...) NULL

annotations.function <- function(object, ...) {
  arguments <- list(...)
  anns <- attr(body(object), "annotations")
  if(length(arguments) == 0) anns
  else switch(type,
              header = anns$header,
              formals = anns$formals,
              body = anns$body,
              footer = anns$footer)
  
}

`annotations<-` <- function(object, type, value) {
  UseMethod("annotations<-", object)
}

`annotations<-.default` <- function(object, ...) {
  attr(object, "annotations") <- list(...)[[1]]
  object
}

`annotations<-.function` <- function(object, ...) {
  arguments <- list(...)
  if(length(arguments) == 1) {
    attr(body(object), "annotations") <- arguments[[1]]
  }
  else {
    value <- arguments[[2]]
    type <- arguments[[1]]
    switch(type,
           header = attr(body(object), "annotations")[["header"]] <- value,
           formals = attr(body(object), "annotations")[["formals"]] <- value,
           body = attr(body(object), "annotations")[["body"]] <- value,
           footer = attr(body(object), "annotations")[["footer"]] <- value)
  }
  object
}

is.annotated <- function(object) !is.null(annotations(object))

.annotations.handlers <-
  list("function" = list("header" = .Internal(new.env(TRUE, emptyenv(), 29L)),
                         "formals" = .Internal(new.env(TRUE, emptyenv(), 29L)),
                         "body" = .Internal(new.env(TRUE, emptyenv(), 29L)),
                         "footer" = .Internal(new.env(TRUE, emptyenv(), 29L))))

annotations.create.handler <- function(name,
                                       action,
                                       mode = "individual",
                                       remove = FALSE) {
  handler <- list(name = name,
                  action = action,
                  mode = mode,
                  remove = remove)
  class(handler) <- c("annotations.handler")
  handler
}

annotations.register <- function(class, type, matcher, handler) {
  handler$matcher <- matcher
  assign(handler$name, handler, envir = .annotations.handlers[[class]][[type]])
}

annotations.deregister <- function(id) {
  # TODO
}

annotations.process <- function(env = parent.frame()) {
  for(name in ls(env)) {
    object <- env[[name]]
    hooks <- new.env()
    if(is.annotated(object))
      assign(name, annotations.invoke(object, name, env), env)
  }
}

annotations.invoke <- function(object, name, env) {
  UseMethod("annotations.invoke", object)
}

annotations.invoke.function <- function(object, name, env) {
  mapping <- .annotations.handlers[["function"]]
  all_annotations <- attr(body(object), "annotations")
  attr(object, "annotations") <- NULL

  handle <- function(type) {
    for(name in ls(mapping[[type]])) {
      handler <- mapping[[type]][[name]]
      matcher <- handler$matcher
      action <- handler$action
      mode <- handler$mode
      remove <- handler$remove
      matches <- NULL
      anns <- NULL
      for(annotation in all_annotations[[type]]) {
        match <- matcher(annotation)
        if(!is.null(match)) {
          if(mode != "digest")
            object <- action(object, name, env, match)
          else
            matches <- append(matches, match)
          if(mode == "once")
            break
        } else {
            anns <- append(anns, annotation)
        }
      }
      if(!is.null(matches))
        object <<- action(object, name, env, matches)
      if(remove)
        all_annotations[[type]] <<- anns
    }
    object
  }
  handle("header")
  handle("formals")
  handle("body")
  handle("footer")
  attr(body(object), "annotations") <- all_annotations
  object
}

print.annotations.function <- function(annotations) {
  print(paste0("header => ", paste0(annotations$header)))
  print(paste0("formals => ", paste0(annotations$formals)))
  print(paste0("body => ", paste0(annotations$body)))
}
