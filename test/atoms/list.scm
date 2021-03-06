;OUTPUT: ()
(println '())

;OUTPUT: (1 2)
(println '(1 2))

;OUTPUT: 1
(println (car '(1 2)))

;OUTPUT: (2 3)
(println (cdr '(1 2 3)))

;OUTPUT: (1 2)
(println (cons 1 '(2)))

;OUTPUT: (1 . 2)
(println (cons 1 2))

;OUTPUT: 2
(println (cdr '(1 . 2)))

;OUTPUT: (1 2 3 4)
(println (list 1 2 3 4))