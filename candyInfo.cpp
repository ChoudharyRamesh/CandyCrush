#include "candyInfo.h"
#include<QRandomGenerator>

QList<QString> CandyInfo::types =  { "Original" , "StripedV", "StripedH" , "Wrapped" , "ColorBomb" ,"Any" };
QList<QString> CandyInfo::colors  = { "Blue" , "Green" , "Orange" , "Purple" , "Red" , "Yellow"  };



CandyInfo::CandyInfo(Color color, Type type){
    this->type = type;
    this->color = color;
}

QString CandyInfo::getPath() const {
    if(type==ColorBomb) return ":/img/ColorBomb/ColorBomb.png";
    else  return ":/img/"+types[type]+"/"+colors[color]+".png";
}

CandyInfo CandyInfo::randomCandy(Type type)
{
    int color =   QRandomGenerator::global()->bounded(6);
    if(type!=Any){
        return CandyInfo(CandyInfo::Color(color) , type);
    }
    else{
        int type =  QRandomGenerator::global()->bounded(5);
        return CandyInfo(CandyInfo::Color(color) , CandyInfo::Type(type));
    }
}
