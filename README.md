# Keccak/SHA3
---

My own implementation of Keccak, derived from Markku-Junahi O.  Saarinen's
[implementation](http://www.mjos.fi/dist/readable_keccak.tgz), but with const
correctness. I will probably extend it to a keccaksum program sometime later.

Like Saarinen's README said, this code is not at all optimized in assembly so do
not expect it to run as fast as a production ready piece of code. This is simply
so that others may get a slightly more "const correct" piece of code that
essentially does the same thing as Saarinen's version.
