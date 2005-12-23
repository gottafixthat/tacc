/** ColorListViewItem - A QListViewItem that allows the background color
  * of the item to be specified.
  */

#include <qlistview.h>
#include <qpainter.h>

class ColorListViewItem : public QListViewItem
{
public:
    ColorListViewItem(QListView *Parent, QColor bgcolor);
    ColorListViewItem(QListView *Parent);
    ColorListViewItem(QListViewItem *Parent); 
    ColorListViewItem(QListView *Parent, QColor bgcolor,
		  const char *s1,
		  const char *s2 = 0,
		  const char *s3 = 0,
		  const char *s4 = 0,
		  const char *s5 = 0,
		  const char *s6 = 0,
		  const char *s7 = 0,
		  const char *s8 = 0);
    ColorListViewItem(QListViewItem *Parent,
		  const char *s1,
		  const char *s2 = 0,
		  const char *s3 = 0,
		  const char *s4 = 0,
		  const char *s5 = 0,
		  const char *s6 = 0,
		  const char *s7 = 0,
		  const char *s8 = 0);
    virtual ~ColorListViewItem();

    virtual void paintFocus(QPainter *p,
			    const QColorGroup &cg,
			    const QRect &r);
    
    virtual void    paintCell(QPainter *p, const QColorGroup &cg,
                        int col, int width, int align);

    virtual void    setColor(const QColor &RGB);
                        
private:
    QColorGroup ColorGroup;
    
    void initialize(QColor bgcolor);
};

