#  File src/library/base/R/Bessel.R
#  Part of the R package, https://www.R-project.org
#
#  Copyright (C) 1995-2012 The R Core Team
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  A copy of the GNU General Public License is available at
#  https://www.R-project.org/Licenses/

annotations <- function(object, ...) UseMethod("annotations", object)
annotations.default <- function(object) NULL
annotations.environment <- function(env) attr(env, "annotations")
annotations.function <- function(fun, ...)
{
    anns <- attr(body(fun), "annotations")
    if(is.null(anns)) return(NULL)
    args <- list(...)
    if(length(args) == 0)
      stop("missing annotation type argument for function object.")
    switch(args[[1]],
           header = anns$header,
           formals = anns$formals,
           body = anns$body,
           stop(sprintf("invalid annotation type '%s' for function objects.",
                        args[[1]])))
}

`annotations<-` <- function(object, ..., value) {
  UseMethod("annotations<-", object)
}
`annotations<-.default` <- function(object, ..., value)
  stop(sprintf("cannot annotate %s objects.", mode(object)))
`annotations<-.environment` <- function(env, value)
{
    attr(env, "annotations") <- value
    env
}
`annotations<-.function` <- function(fun, ..., value)
{
    anns <- attr(body(fun), "annotations")
    if(is.null(anns))
	attr(body(fun), "annotations") <- list(header = NULL,
						  formals = NULL,
						  body = NULL)
    args <- list(...)
    if(length(args) == 0)
      stop("missing annotation type argument for the function being annotated.")
    switch(args[[1]],
           header = attr(body(fun), "annotations")$header <- value,
           formals = attr(body(fun), "annotations")$formals <- value,
           body = attr(body(fun), "annotations")$body <- value,
           stop(sprintf("invalid annotation type '%s' for function objects.",
                        args[[1]])))
    fun
}

is.annotated <- function(object, ...) !is.null(annotations(object, ...))
