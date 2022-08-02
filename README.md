comparison of multiplicative inverses for odd numbers in mod 2<sup>n</sup>

The mnv if and noif algorithms are similar to each other.  It's just that one does things without the branch.

The mnv boi algorithm tried.

I assume the Euler algorithm is how it's implemented elsewhere, but I did re-create it from Euler's theorem with the totient function.

There are two Extended Euclidian algorithms present.  Both are from Wikipedia, and I had no part in designing them.

<hr/>

For the Euler algorithm, there are multiple looping options.  They should all produce identical results (for odd-number inputs), but the "normal" for loop (whose condition is `i < 32`) was fastest on my machine.

i forget if i reaches 32 normally at all
