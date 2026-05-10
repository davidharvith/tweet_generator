# Tweet Generator

Tweet Generator is a C project that builds a first-order Markov chain from a text corpus and uses it to generate new tweet-like sequences. It was written to practice low-level data structures, probabilistic text generation, and clean command-line program design.

## What it does

- Reads a training file of tokenized text
- Builds a Markov chain over word-to-word transitions
- Samples the chain to generate randomized output sentences
- Supports an optional limit on how much of the input corpus is processed

## Build

The project is configured with CMake. From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

This produces the executable defined in the CMake target.

## Run

The program expects a seed, the number of tweets to generate, and an input file. It also supports an optional fourth argument to limit how many words are read from the input.

```bash
./build/ex3 <seed> <number_of_tweets> <input_file> [word_limit]
```

Example:

```bash
./build/ex3 42 5 data/justdoit_tweets.txt
```

## Input format

The generator works best with plain text where sentences are separated by punctuation and words are space-delimited. Each generated tweet stops when the chain reaches a word ending in a period or when the maximum output length is reached.

## Implementation notes

The project is split into a linked-list implementation, a Markov-chain engine, and the CLI entry point. The chain tracks transition frequencies so that words with higher observed probability are sampled more often during generation.
