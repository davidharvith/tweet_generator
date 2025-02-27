#include "markov_chain.h"
#define TWEET_LIMIT 1000
#define WORD_LIMIT 100
#define OUT_PUT_LIMIT 20
#define WITH_LIMIT 5
#define WITH_OUT_LIMIT 4
#define NUM_ARG_ERR "Usage: this program only runs with 3 or 4 arguments!"
#define SEED_NUM 1
#define TWEET_NUM 2
#define FILE_NUM 3
#define LIMIT_NUM 4
#define BASE 10
#define FILE_ERR "Error: invalid file path!"
#include <stdlib.h>








bool deal_with_word(MarkovChain *markov_chain, char *word1, char *word2)
{
  Node *node1 = add_to_database (markov_chain, word1);
  if (!node1)
    {
      return false;
    }
  Node *node2 = NULL;
  if(word2)
    {
      node2 = add_to_database (markov_chain, word2);
      if(!node2)
        {
          return false;
        }
    }


  if(word2)
  {
    add_node_to_frequencies_list (node1->data, node2->data);
  }
  return true;
}

int fill_database_no_limit(FILE *fp, MarkovChain
*markov_chain)
{
  {

    char line[TWEET_LIMIT];
    while ((fgets (line, TWEET_LIMIT, fp) != NULL))
      {

        char *cur_word;
        char *next_word;
        cur_word = strtok (line, " \r\n");
        while (cur_word)
          {
            if (strcmp (&cur_word[strlen (cur_word) - 1], ".") == 0)
              {
                if (!deal_with_word (markov_chain, cur_word, NULL))
                  {
                    return 0;
                  }
                cur_word = strtok (NULL, " \r\n");
              }
            else
              {
                next_word = strtok (NULL, " \r\n");

                if (!deal_with_word (markov_chain, cur_word, next_word))
                  {
                    return 0;
                  }
                cur_word = next_word;
              }
          }
      }
    return 1;
  }
}

int fill_database_with_limit(FILE *fp, MarkovChain
*markov_chain, int limit)
{

  int i = 0;
  char line[TWEET_LIMIT];
  while ((fgets (line, TWEET_LIMIT, fp) != NULL) && (i<limit))
    {

      char *cur_word;
      char *next_word;
      cur_word = strtok(line, " \r\n");
      while(cur_word &&i<limit){

        i++;
          if(strcmp(&cur_word[strlen (cur_word)-1],".") == 0)
            {
              if(!deal_with_word (markov_chain,cur_word ,NULL))
                {
                  return 0;
                }
              cur_word = strtok(NULL, " \r\n");
            }
          else
            {
              next_word = strtok(NULL, " \r\n");

              if(!deal_with_word (markov_chain,cur_word ,next_word))
                {
                  return 0;
                }
              cur_word = next_word;
            }
        }
    }
  return 1;
}

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  if(words_to_read==0)
    {
      return fill_database_no_limit (fp, markov_chain);
    }
  return fill_database_with_limit (fp, markov_chain, words_to_read);
}

int check_num_args(int argv)
{
  if(argv == WITH_LIMIT)
    {
      return WITH_LIMIT;
    }
  if(argv == WITH_OUT_LIMIT)
    {
      return WITH_OUT_LIMIT;
    }
  printf (NUM_ARG_ERR);
  return 0;
}

void initiat_markov_chain(MarkovChain **p_markov_chain)
{
  LinkedList *link_list = malloc(sizeof(LinkedList));
  (*p_markov_chain)->database = link_list;
  link_list->size = 0;
  link_list->first = NULL;
  link_list->last = NULL;
}

int main(int argc, char *argv[])
{
  unsigned int seed = strtol(argv[SEED_NUM], NULL, BASE);
  srand (seed);
  int num_args = check_num_args (argc);
  if(num_args== 0)
    {
      return EXIT_FAILURE;
    }
  int limit = 0;

  if(num_args == WITH_LIMIT)
  {
    limit = strtol(argv[LIMIT_NUM], NULL, BASE);
  }
  FILE *infile = fopen (argv[FILE_NUM], "r");
  if(infile == NULL)
    {
      printf (FILE_ERR);
      return EXIT_FAILURE;
    }
  MarkovChain *markov_chain = malloc (sizeof(MarkovChain));
  if(markov_chain == NULL)
    {
      printf (ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }
  initiat_markov_chain (&markov_chain);
  fill_database(infile,limit , markov_chain);
  for(int i = 1; i < strtol (argv[TWEET_NUM], NULL, BASE)+1; i++)
    {
      printf ("tweet %d ", i);
      generate_tweet (markov_chain, NULL, OUT_PUT_LIMIT);
      printf ("\n");
    }
  free_database (&markov_chain);

  }