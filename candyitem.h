#ifndef CANDYITEM_H
#define CANDYITEM_H

#include<QQuickItem>
#include<QSGSimpleTextureNode>
#include<QSGTexture>
#include<candyInfo.h>

struct Texture{
    Texture(){}
    Texture(QSGTexture * texture,QSize imageSize){
        this->texture = texture;
        this->imageSize = imageSize;
    }
    QSGTexture * texture = nullptr;
    QSize imageSize;
};

class CandyItem : public QQuickItem
{
    Q_OBJECT

public:
    CandyItem(const CandyInfo & _candy, const QPointF & pos, int size);
    void setCandy(const CandyInfo & _candy);
    int column();
    int row();
    CandyInfo candyInfo;

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

private:
    QSize imageSize;
    QSGSimpleTextureNode * textureNode;
    QList<Texture> textures;
    QSGTexture *getTexture();
};


#endif // CANDYITEM_H
