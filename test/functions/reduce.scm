;OUTPUT: 15
(println (reduce + 0 '(1 2 3 4 5)))

;OUTPUT: 20
(println (reduce + 5 '(1 2 3 4 5)))

(let ((add (lambda (x y) (+ x y))))
    ;OUTPUT: 15
    (println (reduce add 0 '(1 2 3 4 5))))