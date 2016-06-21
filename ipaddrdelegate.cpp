#include "ipaddrdelegate.h"
#include <QLineEdit>

IpAddrDelegate::IpAddrDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    
}

QWidget*IpAddrDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setInputMask("000.000.000.000;_");
    return editor;
}

void IpAddrDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();  
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);  
    lineEdit->setText(text); 
}

void IpAddrDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit *lineEdit =  qobject_cast<QLineEdit*>(editor);  
    QString text = lineEdit->text();  
    model->setData(index, text, Qt::EditRole);  
}

void IpAddrDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);  
}
