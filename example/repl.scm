;; Basic REPL.

(define repl
    (lambda () 
        (begin
            (print ">> ")
            (let ((result (eval (read))))
                (begin
                    (print "==> ")
                    (println result)
                    (repl))))))

(println "Scheme REPL, by Sean Gillespie")
(repl)