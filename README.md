## Presentation on [Hana][] for [C++Now][] 2015

This repository contains my [reveal.js][]-based presentation on Hana for
C++Now 2015.

## Basic usage
Go to https://ldionne.github.io/cppnow-2015-hana or open `index.html` with
your browser.

## Advanced usage
From the root of the repository,
```sh
npm install
grunt serve &
```

and then connect to `localhost:8000` to view locally.

## Notes to my future self
`index.html` is generated from `index.in.html`. To generate `index.html`,
```sh
mkdir build
cd build
cmake ..
make index
```

<!-- Links -->
[C++Now]: http://cppnow.org
[Hana]: https://github.com/ldionne/hana
[reveal.js]: https://github.com/hakimel/reveal.js
