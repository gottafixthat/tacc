
#include <qapplication.h>
#include <qpainter.h>
#include "RListViewItem.h"

RListViewItem::RListViewItem(QListView *Parent, int oddColor) : QListViewItem(Parent)
{
    initialize(oddColor);
}


RListViewItem::RListViewItem(
    QListView *Parent)
    : QListViewItem(Parent)
{
    initialize(0);
}

RListViewItem::RListViewItem(
    QListViewItem *Parent)
    : QListViewItem(Parent)
{
    initialize(0);
}

RListViewItem::RListViewItem(
    QListView *Parent, int oddColor,
    const char *s1,
    const char *s2,
    const char *s3,
    const char *s4,
    const char *s5,
    const char *s6,
    const char *s7,
    const char *s8)
    : QListViewItem(Parent, s1, s2, s3, s4, s5, s6, s7, s8)
{
    initialize(oddColor);
}

RListViewItem::RListViewItem(
    QListViewItem *Parent,
    const char *s1,
    const char *s2,
    const char *s3,
    const char *s4,
    const char *s5,
    const char *s6,
    const char *s7,
    const char *s8)
    : QListViewItem(Parent, s1, s2, s3, s4, s5, s6, s7, s8)
{
    initialize(0);
}



RListViewItem::~RListViewItem()
{
}

void RListViewItem::paintCell(
    QPainter *p,
    const QColorGroup &/*cg*/,
    int col, int width,
    int align)
{
    if( ! p)
	return;

    QListViewItem::paintCell(p, ColorGroup, col, width, align);
}

void
RListViewItem::paintFocus(
    QPainter *p,
    const QColorGroup &cg,
    const QRect &r)
{
    /*
    if(listView()->style() == WindowsStyle)
    {
	// I don't care about the Windows style. I'll leave that to Qt:
	
	QListViewItem::paintFocus(p, cg, r);
    }
    else
    {
    */
 	if(isSelected())
 	    p->setPen(cg.base());
 	else
 	    p->setPen(cg.text());

	p->drawWinFocusRect( r );
    /*
    }
    */
}

void RListViewItem::initialize(int oddColor)
{
    QColorGroup  tmpCG = QApplication::palette().normal();
    QColor       oddRGB;
    QColor       evnRGB;
    
    oddRGB.setRgb(255, 255, 255);       // White
    evnRGB.setRgb(153, 204, 204);       // A light green
    
    if (oddColor) {
        ColorGroup = QColorGroup(tmpCG.foreground(), tmpCG.background(),
                                     tmpCG.light(), tmpCG.dark(), tmpCG.mid(),
                                     tmpCG.text(), oddRGB);
    } else {
        ColorGroup = QColorGroup(tmpCG.foreground(), tmpCG.background(),
                                     tmpCG.light(), tmpCG.dark(), tmpCG.mid(),
                                     tmpCG.text(), evnRGB);
    }
}

void RListViewItem::setColor(const QColor &RGB)
{
    QColorGroup  tmpCG = QApplication::palette().normal();
    
    ColorGroup = QColorGroup(tmpCG.foreground(), tmpCG.background(),
                             tmpCG.light(), tmpCG.dark(), tmpCG.mid(),
                             tmpCG.text(), RGB);
}
