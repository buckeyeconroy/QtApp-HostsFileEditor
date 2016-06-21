#include "domaindelegate.h"
#include <QRegExpValidator>
#include <QLineEdit>

DomainDelegate::DomainDelegate(QObject* parent)
{
    
}

QWidget*DomainDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp reg("[\\w\.-]+");
    editor->setValidator(new QRegExpValidator(reg, parent));
    return editor;
}

void DomainDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();  
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);  
    lineEdit->setText(text); 
}

void DomainDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit *lineEdit =  qobject_cast<QLineEdit*>(editor);  
    QString text = lineEdit->text();  
    model->setData(index, text, Qt::EditRole);  
}

void DomainDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect); 
}
