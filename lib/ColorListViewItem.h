/** ColorListViewItem - A QListViewItem that allows the background color
  * of the item to be specified.
  */

#include <Qt3Support/q3listview.h>
#include <QtGui/QPainter>

class ColorListViewItem : public Q3ListViewItem
{
public:
    ColorListViewItem(Q3ListView *Parent, QColor bgcolor);
    ColorListViewItem(Q3ListView *Parent);
    ColorListViewItem(Q3ListViewItem *Parent); 
    ColorListViewItem(Q3ListView *Parent, QColor bgcolor,
		  const char *s1,
		  const char *s2 = 0,
		  const char *s3 = 0,
		  const char *s4 = 0,
		  const char *s5 = 0,
		  const char *s6 = 0,
		  const char *s7 = 0,
		  const char *s8 = 0);
    ColorListViewItem(Q3ListViewItem *Parent,
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

