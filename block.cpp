
#include "block.h"

Block::Block(unsigned int prev_hash,int height,int difficulty,int nonce,int hash,int relayed_by)
{
    this->prev_hash=prev_hash;
    this->height=height;
    this->difficulty=difficulty;
    this->nonce;
    this->hash;
    this->relayed_by;
}
unsigned int Block::get_hash()
 {
    return hash;
 }
int Block::get_difficulty()
{
    return difficulty;
}
int Block::get_height()
{
    return height;
}
        
    

