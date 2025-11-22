## Coding standards

In C++ files, always insert `namespace cy=cutty;` near the top of the file, and use `cy` instead of `cutty` where possible. This is simply for consistency.

Code is formatted using the Microsoft clangd formatter.

All tests pass, and this includes tests on Actions running on all supported platforms.

