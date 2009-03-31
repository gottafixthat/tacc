/*
** RListViewItem  - Register ListView Item - allows the overriding of colors
**                  within a QListView.
*/

#include <Qt3Support/q3listview.h>

class RListViewItem : public Q3ListViewItem
{
public:
    RListViewItem(Q3ListView *Parent, int oddColor);
    RListViewItem(Q3ListView *Parent);
    RListViewItem(Q3ListViewItem *Parent); 
    RListViewItem(Q3ListView *Parent, int oddColor,
		  const char *s1,
		  const char *s2 = 0,
		  const char *s3 = 0,
		  const char *s4 = 0,
		  const char *s5 = 0,
		  const char *s6 = 0,
		  const char *s7 = 0,
		  const char *s8 = 0);
    RListViewItem(Q3ListViewItem *Parent,
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

