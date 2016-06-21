#ifndef IPADDRDELEGATE_H
#define IPADDRDELEGATE_H

#include <QStyledItemDelegate>

class IpAddrDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    IpAddrDelegate(QObject *parent = Q_NULLPTR);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // IPADDRDELEGATE_H
