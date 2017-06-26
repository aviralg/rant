### Tests of function and environment annotations.

## environment annotations
@@:x <- 23
@@:function(y) { y + 1 }

annotations(environment())

@@:"annotation1"
@@:NULL
annotations(environment())

@@:symbol
@@:-23
@@:+89

annotations(environment())

@@:for(i in 1:10) i + 1
@@:list(x=1, y=2, z=3)$x
@@:paste0("Hello", "World", sep=" ")
@@:@:pure function(@:numeric x) @:numeric { x + 1 }

annotations(environment())

##function annotations

subtracter <-
    @:author("Scooby", "Doo", "scooby-doo@mystery.machine")
    function(@:numeric x #comment
            ,@:numeric y #another comment
             #yet another comment
             ) #comment 1
             # comment2
             #can have any number of comments here
             @:numeric
    {
        x - y
    }

annotations(subtracter, "header")
annotations(subtracter, "formals")
annotations(subtracter, "body")

annotations(environment())

## no annotations
f1 <- function(x) { x }

annotations(f1, "header")
is.annotated(f1, "header")

annotations(f1, "formals")
is.annotated(f1, "formals")

annotations(f1, "body")
is.annotated(f1, "body")

## function header annotations

# single header annotation
f2 <- @:header function(x) { x }

is.annotated(f2, "header")
annotations(f2, "header")

is.annotated(f2, "formals")
annotations(f2, "formals")

is.annotated(f2, "body")
annotations(f2, "body")

# multiple header annotations
f3 <- @:header1
@:header2 @:header3
function(x)
{
    x
}

is.annotated(f3, "header")
annotations(f3, "header")

is.annotated(f3, "formals")
annotations(f3, "formals")

is.annotated(f3, "body")
annotations(f3, "body")

## function formals annotations

# single formals annotation
f4 <- function(@:arg1 x) @:body { x }

is.annotated(f4, "header")
annotations(f4, "header")

is.annotated(f4, "formals")
annotations(f4, "formals")

is.annotated(f4, "body")
annotations(f4, "body")

# multiple formals annotations
f5 <- function(@:ann1 @:ann2 @:ann3 x,
               y,
               @:ann1
               @:ann2 z, @:ann1
               @:ann3
               a) { a + x + y + z }

is.annotated(f5, "header")
annotations(f5, "header")

is.annotated(f5, "formals")
annotations(f5, "formals")

is.annotated(f5, "body")
annotations(f5, "body")

## function body annotations

# single body annotation
f6 <- function(x) @:body { x }

is.annotated(f6, "header")
annotations(f6, "header")

is.annotated(f6, "formals")
annotations(f6, "formals")

is.annotated(f6, "body")
annotations(f6, "body")

# multiple body annotations
f7 <- function(x) @:body1
@:body2
@:body3 {
    x
}

is.annotated(f7, "header")
annotations(f7, "header")

is.annotated(f7, "formals")
annotations(f7, "formals")

is.annotated(f7, "body")
annotations(f7, "body")

## function header and formals annotations
f8 <- @:header1
@:header2 @:header3 function(@:ann1 @:ann2 @:ann3 x,
               y,
               @:ann1
               @:ann2 z, @:ann1
               @:ann3
               a) {
  a + x + y + z
}

is.annotated(f8, "header")
annotations(f8, "header")

is.annotated(f8, "formals")
annotations(f8, "formals")

is.annotated(f8, "body")
annotations(f8, "body")

## function header and body annotations
f9 <- @:header1
    @:header2 @:header3
    function(a, x, y, z)
    @:body1
    @:body2
    @:body3 {
  a + x + y + z
}

is.annotated(f9, "header")
annotations(f9, "header")

is.annotated(f9, "formals")
annotations(f9, "formals")

is.annotated(f9, "body")
annotations(f9, "body")

## function formals and body annotations
f10 <- function(@:ann1 @:ann2 @:ann3 x,
                y,
                @:ann1
                @:ann2 z, @:ann1
                @:ann3
                a) @:body1
    @:body2
    @:body3 {
      a + x + y + z
}

is.annotated(f10, "header")
annotations(f10, "header")

is.annotated(f10, "formals")
annotations(f10, "formals")

is.annotated(f10, "body")
annotations(f10, "body")

## function header, formals and body annotations
f11 <- @:header1
    @:header2 @:header3
    function(@:ann1 @:ann2 @:ann3 x,
               y,
               @:ann1
               @:ann2 z, @:ann1
               @:ann3
               a) @:body1
    @:body2
    @:body3 {
      a + x + y + z
}

is.annotated(f11, "header")
annotations(f11, "header")

is.annotated(f11, "formals")
annotations(f11, "formals")

is.annotated(f11, "body")
annotations(f11, "body")

## setting environment annotations
e <- environment()
is.annotated(e)
annotations(e)
annotations(e) <- list(quote(1 + 2), quote(hello))
is.annotated(e)
annotations(e)
annotations(e) <- NULL
is.annotated(e)
annotations(e)

## setting function annotations
is.annotated(f11, "header")
annotations(f11, "header")
annotations(f11, "header") <- list(quote(header_changed))
is.annotated(f11, "header")
annotations(f11, "header")
annotations(f11, "header") <- NULL
is.annotated(f11, "header")
annotations(f11, "header")

is.annotated(f11, "body")
annotations(f11, "body")
annotations(f11, "body") <- list(quote({12 + 13}))
is.annotated(f11, "body")
annotations(f11, "body")
annotations(f11, "body") <- NULL
is.annotated(f11, "body")
annotations(f11, "body")

is.annotated(f11, "formals")
annotations(f11, "formals")
annotations(f11, "formals") <- list(x = list(quote(ann_x)))
is.annotated(f11, "formals")
annotations(f11, "formals")
annotations(f11, "formals") <- NULL
is.annotated(f11, "formals")
annotations(f11, "formals")


## getting function annotations without type argument
stopifnot(inherits(try(annotations(f11)), "try-error"))

## getting function annotations with invalid type argument
stopifnot(inherits(try(annotations(f11, "abcd")), "try-error"))

## setting function annotations without type argument
stopifnot(inherits(try(annotations(f11) <- NULL), "try-error"))

## setting function annotations with invalid type argument
stopifnot(inherits(try(annotations(f11, "efgt") <- NULL), "try-error"))

## getting annotations from non-function and non-environment objects
annotations(2)
annotations("hello")
annotations(list(1, 2, 3))

## setting annotations on non-function and non-environment objects
x <- 2
stopifnot(
  inherits(
    try(annotations(x) <- list(quote(hello), quote(world))),
    "try-error"))

y <- "i am a string"
stopifnot(
  inherits(
    try(annotations(y) <- list(quote(hello), quote(world))),
    "try-error"))

z <- list(1, 3, 5)
stopifnot(
  inherits(
    try(annotations(z) <- list(quote(hello), quote(world))),
    "try-error"))
