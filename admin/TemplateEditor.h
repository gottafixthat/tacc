/**
 * TemplateEditor.h - An admin interface to edit email templates stored in the
 * database.
 *
 * Written by R. Marc Lewis
 *   (C)opyright 1998-2009, R. Marc Lewis and Avvatel Corporation
 *   All Rights Reserved
 *
 *   Unpublished work.  No portion of this file may be reproduced in whole
 *   or in part by any means, electronic or otherwise, without the express
 *   written consent of Avvatel Corporation and R. Marc Lewis.
 */

#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <qpushbutton.h>
#include <q3listview.h>
#include <q3textedit.h>
#include <qlineedit.h>
#include <TAAWidget.h>

class TemplateEditor : public TAAWidget
{
    Q_OBJECT

public:

    TemplateEditor(QWidget* parent = NULL, const char* name = NULL);
    virtual ~TemplateEditor();
    
    void    refreshTemplates(void);

protected:
    QPushButton     *deleteButton;
    QPushButton     *saveButton;
    Q3ListView       *templateList;
    QLineEdit       *templateName;
    QLineEdit       *subject;
    Q3TextEdit       *editor;

protected slots:
    virtual void    templateSelected(Q3ListViewItem *curItem);
    virtual void    newTemplate();
    virtual void    saveTemplate();
    virtual void    deleteTemplate();
    void            updateTemplate();
    void            textItemChanged();
    void            textItemChanged(const QString &);

private:
    bool            itemChanged;
    long            myTemplateID;
    QString         origName;
    QString         origSubject;
    QString         origBody;
    void            loadTemplates();

};
#endif
