This is a random tweet generator implemented in C using Markov Chains. The program reads a text file filled with tweets, builds a Markov model, and generates random tweets based on the training data.

To compile run:
gcc src/*.c -o bin/tweet_generator

To run:
./bin/tweet_generator data/tweets.txt
