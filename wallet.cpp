#include"wallet.h"
#include<iostream>
#include"CSVReader.h"
using namespace std;

Wallet :: Wallet(){

}
//**insert cuurency to the wallet */
void Wallet:: insertCurrency(string type, double amount){
    double balance;
    if(amount < 0){
        throw exception{};
    }
    if(currencies.count(type) == 0) // not there yet
    {
        balance = 0;
    }
    else{  // money is there in the walley
        cout<<"No cuurency for "<<type <<endl;
        balance= currencies[type];
    }
    balance +=amount;
    currencies[type] = balance;

}
bool Wallet::removeCurrency(string type , double amount){
    double balance;
    if(amount < 0){
        return false;
    }
    if(currencies.count(type) == 0) // not there yet
    {
        return false;
    }
    else{  // do we have enough
        if(containsCurrency(type,amount)){  // we have enough
            cout<<"Removing "<<type <<" : "<<amount<<endl;
            currencies[type] -= amount;
            return true;
        }
        else{ // they have it but not enough
            return false;
        }
        
    }
}
/** check if the wallet comntains this much cuurency or not*/ 
bool Wallet::containsCurrency(string type, double amount){
    if(currencies.count(type) == 0) // not there yet
    {
        return  false;
    }
    else{
        return currencies[type] >= amount;
    }
    
}
string Wallet::toString(){
    string s;
    for(pair<string,double>pair : currencies){
        string currency = pair.first;
        double amount = pair.second;
        s += currency +" : "+to_string(amount)+"\n";
         
    }
    return s;
}

bool Wallet::canFullfillOrder(OrderBookEntry order){
    vector<string> currs =CSVReader ::tokenise(order.product,'/');
    //ask
    if(order.ordertype == OrderBookType::ask){
        double amount = order.amount;
        string currency = currs[0];
        cout<<"Wallet::canFullfillOrders "<<currency<<" : "<<amount<<endl;
        return containsCurrency(currency,amount);

    }

    //bid
    if(order.ordertype == OrderBookType :: bid){
        double amount = order.amount * order.price;
        string currency = currs[1];
        cout<<"Wallet::canFullfillOrders "<<currency<<" : "<<amount<<endl;
        return containsCurrency(currency,amount);
    }
    return false;
}

void Wallet::processSale(OrderBookEntry & sale){
    vector<string> currs =CSVReader ::tokenise(sale.product,'/');
    //ask
    if(sale.ordertype == OrderBookType::asksale){
        double outgoingAmount = sale.amount;
        string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

    }

    //bid
    if(sale.ordertype == OrderBookType :: bidsale){
        double incomingAmount = sale.amount;
        string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

    }
}