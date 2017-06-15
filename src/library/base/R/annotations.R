annotations <- function(object, type) {
  UseMethod("annotations", object)
}

annotations.default <- function(object, ...) NULL

annotations.function <- function(object, ...) {
  arguments <- list(...)
  f <- if(length(arguments) == 0) annotations.function.all
       else switch(type,
                   prefix = annotations.function.prefix,
                   formals = annotations.function.formals,
                   infix = annotations.function.infix,
                   postfix = annotaions.function.postfix)
  f(object)
}

annotations.function.all <- function(object) {
  attr(body(object), "annotations")
}

annotations.function.prefix <- function(object) {
  annotations.function.all(object)$prefix
}

annotations.function.formals <- function(object) {
  annotations.function.all(object)$formals
}

annotations.function.infix <- function(object) {
  annotations.function.all(object)$body
}

annotations.function.postfix <- function(object) {
  annotations.function.all(object)$postfix
}

`annotations<-` <- function(object, type, value) {
  UseMethod("annotations<-", object)
}

`annotations<-.default` <- function(object, ...) {
  object
}

`annotations<-.function` <- function(object, ...) {
  arguments <- list(...)
  if(length(arguments) == 1) {
    value <- arguments[[1]]
    f <- `annotations<-.function.all`
  }
  else {
    value <- arguments[[2]]
    type <- arguments[[1]]
    f <- switch(type,
                prefix = `annotations<-.function.prefix`,
                formals = `annotations<-.function.formals`,
                infix = `annotations<-.function.infix`,
                postfix = `annotaions<-.function.postfix`)
  }
  f(object, value)
}

`annotations<-.function.all` <- function(object, value) {
  attr(body(object), "annotations") <- value
  object
}

`annotations<-.function.prefix` <- function(object, value) {
  attr(body(object), "annotations")[["prefix"]] <- value
  object
}

`annotations<-.function.formals` <- function(object, value) {
  attr(body(object), "annotations")[["formals"]] <- value
  object
}

`annotations<-.function.infix` <- function(object, value) {
  attr(body(object), "annotations")[["infix"]] <- value
  object
}

`annotations<-.function.postfix` <- function(object, value) {
  attr(body(object), "annotations")[["postfix"]] <- value
  object
}
