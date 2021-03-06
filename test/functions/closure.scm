(define immutable-capture
    (let ((x 42))
        (lambda (q) (+ q x))))

(define mutable-capture
    (let ((x 42))
        (lambda (v) 
            (begin
                (set! x v)
                x))))

;OUTPUT: 43
(println (immutable-capture 1))

;OUTPUT: 1
(println (mutable-capture 1))