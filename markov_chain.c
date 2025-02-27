#include "markov_chain.h"
#define TWEET_LIMIT 20
#define EXTRA_ROOM 2
/**
 * Get random number between 0 and max_number [0, max_number)
 * @param max_number maximal number to return
 * @return random number
 */
int get_random_number(int max_number)
{
  return rand() % max_number;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
  Node *potential = look_in_database(data_ptr, markov_chain);
  if(potential != NULL)
    {
      return potential; // if in database return it
    }
  MarkovNode *to_add = create_node (data_ptr, &markov_chain);
  if(to_add==NULL)
    {
      return NULL;
    }
  int test = add(markov_chain->database, to_add);
  if(test == 1)
    {
      free_database (&markov_chain);
      return NULL;
    }
  return markov_chain->database->last; // if not create a new one and add it
}


Node *look_in_database(char *data_ptr, MarkovChain *markov_chain)
{
  LinkedList *data_base = markov_chain->database;
  unsigned int list_size = data_base->size;
  Node *cur = data_base->first; // get all relavant data from markov_chain
  for (unsigned int i = 0; i < list_size; i++)
    {
      if (strcmp (data_ptr, cur->data->data) == 0) // look for node in chain
        {
          return cur;
        }
      cur = cur->next;
    }
    return NULL;
}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
  return look_in_database (data_ptr, markov_chain);
}


bool add_node_to_frequencies_list(MarkovNode *first_node, MarkovNode *second_node)
{
  unsigned int list_size = first_node->list_size;
  for(unsigned int i=0; i < list_size; i++)
    {
      if(strcmp(second_node->data,
                first_node->frequencies_list[i].markov_node->data)==0)
        {
          first_node->frequencies_list[i].frequency += 1;
          return true; // if already in list increase freq and return true
        }
    }
    // if not create a MNF to add to list
  MarkovNodeFrequency to_add = (MarkovNodeFrequency) {second_node, 1};
  if(list_size == 0)
    {
      // if it's the first item use malloc
      first_node->frequencies_list = malloc (sizeof (MarkovNodeFrequency));
      if(first_node->frequencies_list == NULL)
        {
          return false;
        }

      first_node->frequencies_list[0] = to_add;
      first_node->list_size+=1;
      return true;
    }
    // else use realloc to make room for it and add it
  first_node->frequencies_list = realloc (first_node->frequencies_list,
                                          (list_size+1)*sizeof(MarkovNodeFrequency));
  if(first_node->frequencies_list == NULL)
    {
      return false;
    }
  first_node->frequencies_list[list_size] = to_add;
  first_node->list_size+=1;
  return true;
}


MarkovNode *create_node(char *data, MarkovChain **p_markov_chain)
{
  MarkovNode *markov_node = malloc (sizeof(MarkovNode)); // dynamicly alloct
  // room for node
  if(markov_node == NULL)
    {
      free_database(p_markov_chain);
      return NULL;
    }// same for its data
  markov_node->data = malloc (strlen (data)+EXTRA_ROOM);
  if(markov_node->data == NULL)
    {
      free_database(p_markov_chain);
      return NULL;
    }// copy data to new node and intiat it with freq size 0
  strcpy(markov_node->data,data);
markov_node->list_size = 0;
return markov_node;
}

void free_database(MarkovChain **p_markov_chain)
{
  unsigned int list_size = (*p_markov_chain)->database->size;
  Node *cur = (*p_markov_chain)->database->first;
  for(unsigned int i = 0; i < list_size; i++)//loop over linked list
    {
      Node *temp = cur;
      if(cur->data->list_size!=0)// if there is freq list just free it
        {
          free (cur->data->frequencies_list);
        }
      cur = cur->next; //advance before loosing access
      free(temp->data->data); // free all layers of dynamic memory
      free(temp->data);
      free(temp);
    }
  free ((*p_markov_chain)->database);//free linked list
  free (*p_markov_chain); // free markove chain
}


MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
  int max = (markov_chain->database->size)-1;
  while(true) // go till finding a sutibale node
    {
      int random = get_random_number (max);
      Node *cur = markov_chain->database->first;
      for(int i = 0;  i < random; i++)
        {
          cur = cur->next; // get node according to random number
        }
      if(cur->data->data[strlen(cur->data->data)-1]!= '.')
        {
          return cur->data; // if it is ok return it, if not go again
        }
    }
}

int get_len_freq_list(MarkovNode *markov_node)
{
  int tot = 0;
  for(int i = 0; i < (int)markov_node->list_size; i ++)
  {
    tot += (int)markov_node->frequencies_list[i].frequency;//for every node
    // in list find how many occurrences it has and add them up
  }
 return tot;
}

MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  int counter = 0;
  int list_len = get_len_freq_list(state_struct_ptr)+1;//get max int
  // to make sure the random number will work
  int random = get_random_number (list_len);
  while(true)//go till success
    {
      random -= state_struct_ptr->frequencies_list[counter].frequency;
      if(random<=0)// deacres from random till we find where it falls
        {
          return state_struct_ptr->frequencies_list[counter].markov_node;
        }
      counter++; // move to next tweet
    }
}


void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{

  char *tweet[TWEET_LIMIT];
  if(first_node == NULL)
    {
      first_node = get_first_random_node (markov_chain);
    }
  MarkovNode *cur = first_node;
  int i = 0;
  for(; i < max_length; i++)
    {
      tweet[i] = cur->data;

      if(tweet[i][strlen (tweet[i])-1] == '.')
        {
          i++;
          break;
        }


      cur = get_next_random_node (cur);

    }
  for(int k = 0; k <i; k++)
    {
      printf ("%s ", tweet[k]);
    }


}
