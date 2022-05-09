#ifndef CANDYINFO_H
#define CANDYINFO_H

#include<QString>
#include<QList>

class CandyInfo{

public:
    enum Color { Blue , Green , Orange , Purple , Red , Yellow };
    enum Type { Original , StripedV, StripedH , Wrapped , ColorBomb ,Any };

    CandyInfo (Color color , Type  type );
    Color color;
    Type type;

    QString getPath()const;

    static CandyInfo randomCandy(CandyInfo::Type type = CandyInfo::Original);

private :
    static QList<QString>  types ;
    static QList<QString>  colors ;
};


#endif // CANDYINFO_H
