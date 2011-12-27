main = print ( take 100 primes )

inc n = n + 1

primes = sieve [2..]
	where
		sieve :: [Int] -> [Int]
		sieve (p:xs) =
			p : sieve [ x | x <- xs, x `mod` p > 0 ]
