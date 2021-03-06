#include "Rounds.h"

Rounds::Rounds(Player *p1, Player *p2, string sourceFilePath):player1(p1), player2(p2), cardFactory(nullptr), roundCount(0){
    cardFactory = new CardFactory(sourceFilePath);
}

void Rounds::start(){
    while(1){
        cout << "=============================================" << endl;
        cout << "ROUND" << roundCount << ": ";
        Round* r = new Round(player1, player2, cardFactory);
        roundList.push_back(r);
        if((winnerName = r->start()) != ""){
            cout << "Winner is: " << winnerName << "!" << endl;
            cout << "Game over" << endl;
            break;            
        }
        roundCount++;
    }
}

Rounds::~Rounds(){
    for(auto &p:roundList){
        delete p;
    }
    roundList.clear();
    delete cardFactory;
}

string Rounds::Round::start(){
    Player *attacker = nullptr;
    Player *victim = nullptr;
    while (player1->passCardCount <= 0 && player2->passCardCount <= 0){
        player1->passCardCount++;
        player2->passCardCount++;
    }
    if(player1->passCardCount){
        cout << "player1 round " << endl;
        attacker = player1;
        victim = player2;
    }
    else if(player2->passCardCount){
        cout << "player2 round " << endl;
        attacker = player2;
        victim = player1;
    }

    attacker->passCardCount--;
    const Card* card = attacker->playCard();

    if(!card)[[unlikely]]{
        cerr << "no card sequence!" << endl;
        exit(1);
    }

    card->showCardInfo();
    card->makeEffect(attacker, victim);
    delete card;
    attacker->attack(victim);

    //give player a card
    attacker->getOneCard(cardFactory->generateCard());
    
    showPlayersMonster();

    if(*victim)[[unlikely]]{
        return attacker->playerName;
    }
    return "";
}

Rounds::Round::Round(Player* p1, Player* p2, CardFactory* cf):player1(p1), player2(p2), cardFactory(cf){
}

void Rounds::Round::showPlayersMonster() const {
    player1->showMonsters();
    player2->showMonsters();
}

