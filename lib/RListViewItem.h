/*
** RListViewItem  - Register ListView Item - allows the overriding of colors
**                  within a QListView.
*/

#include <qlistview.h>

class RListViewItem : public QListViewItem
{
public:
    RListViewItem(QListView *Parent, int oddColor);
    RListViewItem(QListView *Parent);
    RListViewItem(QListViewItem *Parent); 
    RListViewItem(QListView *Parent, int oddColor,
		  const char *s1,
		  const char *s2 = 0,
		  const char *s3 = 0,
		  const char *s4 = 0,
		  const char *s5 = 0,
		  const char *s6 = 0,
		  const char *s7 = 0,
		  const char *s8 = 0);
    RListViewItem(QListViewItem *Parent,
		  const char *s1,
		  const char *s2 = 0,
		  const char *s3 = 0,
		  const char *s4 = 0,
		  const char *s5 = 0,
		  const char *s6 = 0,
		  const char *s7 = 0,
		  const char *s8 = 0);
    virtual ~RListViewItem();

    virtual void paintFocus(QPainter *p,
			    const QColorGroup &cg,
			    const QRect &r);
    
    virtual void    paintCell(QPainter *p, const QColorGroup &cg,
                        int col, int width, int align);

    virtual void    setColor(const QColor &RGB);
                        
private:
    QColorGroup ColorGroup;
    
    void initialize(int oddColor);
};

