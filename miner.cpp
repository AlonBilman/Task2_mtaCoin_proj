
#include "miner.h"



Miner::Miner(int id)
{
    this->id = id;
}

void Miner::update_target_parameters()
{
   difficulty_target=Server::block_chain.front().get_difficulty();
   height_target=Server::block_chain.front().get_height() +1;
   last_hash=Server::block_chain.front().get_hash();
   mask = mask_hash_validation(difficulty_target);
   nonce = 0;
}


unsigned int Miner::calculate_hash_code()
{
    timestamp = std::time(nullptr);
    return hash(height_target,nonce,timestamp,last_hash,id);
}

void Miner::start_mining() //need thread things.
{
    while(true)
    {   
        if(height_target-1 != Server::block_chain.front().get_height()) 
            update_target_parameters();
        
        unsigned int crc_res = calculate_hash_code(); //this also updates the timestamp
        bool hit = mined_success(crc_res);

        if(hit) //update the server "socket" or "mail-box"
        {
            std::cout<<"Miner #"<<id
            <<" mined a new Block #"<<height_target
            <<", With the hash "<<crc_res<<std::endl;
            Server::next_block= Block(last_hash,height_target,difficulty_target,nonce,crc_res,id,static_cast<int>(timestamp)); 
            pthread_cond_signal(&cond);
            sleep(0.1);
        }
        else
            ++nonce;
        //sleep(1);
    } 
}

bool Miner::mined_success(const unsigned int crc_res) const
{
    return ((crc_res & mask) == 0);
}

int Miner::get_id()
{
    return id;
}