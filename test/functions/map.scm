;OUTPUT: (3 4 5 6)
(println (map (lambda (x) (+ x 2)) '(1 2 3 4)))

;OUTPUT: 5
;OUTPUT: 6
;OUTPUT: 7
;OUTPUT: 8
(map println '(5 6 7 8))