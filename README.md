## Presentation on [Hana][] for [C++Now][] 2015

This repository contains my [reveal.js][]-based presentation on Hana for
C++Now 2015.

## Disclaimer
The presentation is not finished yet. This should really be a private repo,
but I only have the free plan on GitHub :-). Don't go around selling the
punch before the conference if you stumble on this.


## Basic usage
Go to https://ldionne.github.io/hana-cppnow-2015 or open `index.html` with
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
