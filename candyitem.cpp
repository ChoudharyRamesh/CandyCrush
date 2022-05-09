#include "candyitem.h"
#include<board.h>
#include<QSGSimpleTextureNode>
#include<QDebug>
#include<QTimer>

CandyItem::CandyItem(const CandyInfo & _candy, const QPointF & pos,int size):
    candyInfo(_candy.color,_candy.type)
{
    setFlag(QQuickItem::ItemHasContents);
    setSize(QSizeF(size,size) );
    setPosition(pos);
    textures.resize(6);
}

void CandyItem::setCandy(const CandyInfo &_candy)
{
    this->candyInfo = _candy;
    textureNode->setTexture(getTexture());
    update();

}

int CandyItem::column(){ return ceil(x()/width()); }

int CandyItem::row(){ return ceil(y()/height()); }



QSGNode * CandyItem::updatePaintNode(QSGNode * rootNode , UpdatePaintNodeData * data )
{
    Q_UNUSED(data)

    if(!rootNode)
    {
        rootNode = new QSGNode;
        textureNode = new QSGSimpleTextureNode();
        textureNode->setTexture(getTexture());
        // textureNode->setOwnsTexture(true);
        textureNode->setFlag(QSGNode::OwnedByParent,true);
        rootNode->appendChildNode(textureNode);
    }


    QSize newRenderSize = imageSize.scaled(width()*0.9,height()*0.9,Qt::KeepAspectRatio);
    QRectF renderRect(width()/2 - newRenderSize.width()/2 ,
                      height()/2- newRenderSize.height()/2
                      ,newRenderSize.width(),newRenderSize.height());

    textureNode->setRect(renderRect);
    return rootNode;
}


QSGTexture *CandyItem::getTexture()
{
    QSGTexture * texture;
    if(textures.at(candyInfo.color).texture ==nullptr)
    {
        QImage image(candyInfo.getPath());
        imageSize = image.size();
        texture = window()->createTextureFromImage(image);
        connect(texture,&QSGTexture::destroyed,this,[](){ qDebug()<<"destroyed"; });
        textures [candyInfo.color] = Texture(texture,imageSize);
    }
    else{

        Texture t = textures.at(candyInfo.color);
        imageSize = t.imageSize;
        texture = t.texture;
    }
    return texture;
}
