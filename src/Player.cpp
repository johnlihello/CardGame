#include "Player.h"
#include "CardFactory.h"

Player::Player(string sourceFilePath, string pN):blood(BLOOD_PLAYER),passCardCount(0), playerName(pN){
    CardFactory *cardFactory = new CardFactory(sourceFilePath);
    while(*cardFactory){
        const Card* card = cardFactory->generateCard();
        cardList.push_back(card);
    }
    delete cardFactory;
}

void Player::getOneCard(const Card* newCard){
    cardList.push_back(newCard);
    cout << playerName << " get a card name is: " << newCard->name << endl;
}

void Player::ListCard() const{
    for(auto &p:cardList){
        cout << p->name << endl;
    }
}

Player::~Player(){
    for(auto &p:cardList){
        delete p;
    }
    for(auto &p:monsterList){
        delete p;
    }
}

void Player::readPlayCardSequence(string sourceFilePath){
    fstream file;
    file.open(sourceFilePath, ios::in);
    if(!file)[[unlikely]]{
        cout << "file path is not found." << endl;
        exit(1);
    }
    int num;
    while(file){
        file >> num;
        playCardSequenceList.push_back(num);
    }
    file.close();
}

const Card* Player::playCard(){
    //no playcard sequence
    if(playCardSequenceList.size() == 0){
        return nullptr;
    }
    int popCardID = *playCardSequenceList.begin();
    cout<< playerName << " pop a Card, Id is: " << popCardID << endl;
    const Card* card = cardList[popCardID];
    //clean
    playCardSequenceList.erase(playCardSequenceList.begin());
    cardList.erase(cardList.begin()+popCardID);
    return card;
}

void Player::attack(Player* victim){
    for(auto &p: monsterList){
        cout << p->name << endl;
        p->attack(victim);
    }
}

void Player::showMonsters() const{
    cout << "++++++++++++++++++++++++" << endl;
    cout << playerName << ": blood is " << blood << endl;
    for(auto &p: monsterList){
        p->showMonsterInfo();
    }
}

void Player::decreaseAllMonstersBlood(int number){
    vector<Monster*>::iterator it;
    for(it=monsterList.begin(); it != monsterList.end();){
        bool isDead = (*it)->decreaseBlood(number);
        //delete this monster
        if(isDead){
            cout << "DEAD:" << (*it)->name << endl;
            delete (*it);
            monsterList.erase(it);
        }
        else{
            ++it;
        }
    }
}

void Player::addMonster(Monster* monster){
    monsterList.push_back(monster);
}

void Player::decreaseBlood(int number){
    cout << "blood: "<< blood << " -> " << blood-number <<endl;
    blood -= number;
}

void Player::stall(int number){
    passCardCount -= number;
}