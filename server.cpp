
#include "server.h"
#include <iostream>

std::list<Block> Server::block_chain;
Block Server::next_block;//first dummy block. start of the chain , height is 1

//create one dummy node 
Server::Server(int difficulty_target) : difficulty_target(difficulty_target)
{
    next_block.set_difficulty(difficulty_target);
    block_chain.push_front(next_block);
    
}

void Server::print_last_block(Block& block_added)
{
   std::cout << "Server: New block added by Miner #" << block_added.get_relayed_by()
                     << ", height(" << block_added.get_height() << "), timestamp(" << block_added.get_timestamp()
                     << "), hash(0x" << std::hex << block_added.get_hash() << std::dec << "), prev_hash(0x"
                     << std::hex << block_added.get_prev_hash() << std::dec << "), nonce(" 
                     << block_added.get_nonce() << ")" << std::endl;
}

bool Server::verify_proof_of_work(Block& block_to_check)
{
    if(block_to_check.get_difficulty()!=difficulty_target)
        return false;
    else if(block_to_check.get_height()!=number_of_blocks+1)
        return false;
    else if(block_to_check.get_prev_hash()!=block_chain.front().get_hash())
        return false;

    unsigned int hash_test = //this calculates the hash, again.. :) 
        hash(block_to_check.get_height(),block_to_check.get_nonce(),static_cast<time_t>(block_to_check.get_timestamp()),
             block_to_check.get_prev_hash(),block_to_check.get_relayed_by());
    
    if(hash_test!=block_to_check.get_hash())
        return false;
    else if(hash_test<=max_hash_calculator(difficulty_target))
        return false;

    return true; //if we survived all the checks.
}


void Server::add_block(Block & block_to_add) //addint to block_chain. making sure that its secure
{
    pthread_mutex_lock(&bl_lock);
    block_chain.push_front(block_to_add);
    ++number_of_blocks;
    //calling the print function 
    print_last_block(block_to_add);
    pthread_mutex_unlock(&bl_lock);
}

void Server::start()
{    
    pthread_mutex_lock(&mutex);

    pthread_cond_wait(&cond,&mutex); //waiting for a block to check

    Block curr_block_to_check = next_block;

    if(verify_proof_of_work(curr_block_to_check))
    {
        add_block(curr_block_to_check);
    }

    pthread_mutex_unlock(&mutex);
}
