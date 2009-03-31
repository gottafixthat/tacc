
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include "ColorListViewItem.h"

ColorListViewItem::ColorListViewItem(Q3ListView *Parent, QColor bgcolor) : Q3ListViewItem(Parent)
{
    initialize(bgcolor);
}


ColorListViewItem::ColorListViewItem(
    Q3ListView *Parent)
    : Q3ListViewItem(Parent)
{
    QColor  defaultcolor(255,255,255);
    initialize(defaultcolor);
}

ColorListViewItem::ColorListViewItem(
    Q3ListViewItem *Parent)
    : Q3ListViewItem(Parent)
{
    QColor  defaultcolor(255,255,255);
    initialize(defaultcolor);
}

ColorListViewItem::ColorListViewItem(
    Q3ListView *Parent, QColor bgcolor,
    const char *s1,
    const char *s2,
    const char *s3,
    const char *s4,
    const char *s5,
    const char *s6,
    const char *s7,
    const char *s8)
    : Q3ListViewItem(Parent, s1, s2, s3, s4, s5, s6, s7, s8)
{
    initialize(bgcolor);
}

ColorListViewItem::ColorListViewItem(
    Q3ListViewItem *Parent,
    const char *s1,
    const char *s2,
    const char *s3,
    const char *s4,
    const char *s5,
    const char *s6,
    const char *s7,
    const char *s8)
    : Q3ListViewItem(Parent, s1, s2, s3, s4, s5, s6, s7, s8)
{
    QColor  defaultcolor(255,255,255);
    initialize(defaultcolor);
}



ColorListViewItem::~ColorListViewItem()
{
}

void ColorListViewItem::paintCell(
    QPainter *p,
    const QColorGroup &/*cg*/,
    int col, int width,
    int align)
{
    if( ! p)
	return;

    Q3ListViewItem::paintCell(p, ColorGroup, col, width, align);
}

void
ColorListViewItem::paintFocus(
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

	//p->drawWinFocusRect( r );
    /*
    }
    */
}

void ColorListViewItem::initialize(QColor bgcolor)
{
    QColorGroup  tmpCG = QApplication::palette().normal();
    ColorGroup = QColorGroup(tmpCG.foreground(), tmpCG.background(),
                                 tmpCG.light(), tmpCG.dark(), tmpCG.mid(),
                                 tmpCG.text(), bgcolor);
}

void ColorListViewItem::setColor(const QColor &RGB)
{
    QColorGroup  tmpCG = QApplication::palette().normal();
    
    ColorGroup = QColorGroup(tmpCG.foreground(), tmpCG.background(),
                             tmpCG.light(), tmpCG.dark(), tmpCG.mid(),
                             tmpCG.text(), RGB);
}
