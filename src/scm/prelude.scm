;; This file (the prelude) is loaded automatically when
;; the interpreter starts up.

;; =================================================
;; Boolean operations
;; =================================================

;; (and) - builtin form
;; (or)  - builtin form

;; Returns the boolean negation of the given value.
(define not
    (lambda (value)
        (if value #f #t)))

;; ==================================================
;; List operations
;; ==================================================

;; (define list?) - intrinsic
;; (define cdr)   - intrinsic
;; (define car)   - intrinsic
;; (define cons)  - intrinsic

;; Predicate that returns true if a list is the null (empty) list,
;; returns false otherwise.
(define null?
    (lambda (list)
        (eq? list '())))

;; Applies a function to a list, yielding a new list.
(define map
    (lambda (f list)
        (if (null? list)
            '()
            (cons (f (car list)) (map f (cdr list))))))

;; Reduces a list by applying a function across all elements.
(define reduce
    (lambda (f zero list)
        (if (null? list)
            zero
            (let ((head (car list)) (tail (cdr list)))
                (f head (reduce f zero tail))))))

(define list
    (lambda (head . tail)
        (cons head tail)))

;; ==================================================
;; Assoc utilities
;; ==================================================

;; Given an associated list, returns the value associated
;; with the key in that list, or #f if the key does not exist
;; in the list.
(define assoc
    (lambda (key alist)
        (if (null? alist)
            #f
            (let ((head (car alist)) (tail (cdr alist)))
                (if (equal? (car head) key)
                    (cdr head)
                    (assoc key tail))))))

;; Returns #t if the given object is a pair, #f otherwise.
(define pair?
    (lambda (p)
        (if (null? p)
            #f
            (and (not (list? (car p))) (not (list? (cdr p)))))))

;; Appends a key-value pair to an associated list.
(define assoc-cons
    (lambda (key value alist)
        (cons (cons key value) alist)))

;; ==================================================
;; Functional utilities
;; ==================================================

;; (define proc?) - intrinsic
;; (define arity) - intrinsic
;; (define is-native-proc) - intrinsic

;; Applies a function to a given value.
(define apply
    (lambda (f x)
        (f x)))