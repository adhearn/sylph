(letrec ((add1 (lambda (x) (primcall + x (quote 1))))
         (double-first (lambda (x y z) (primcall + x x))))
  (add1 (quote 5)))
