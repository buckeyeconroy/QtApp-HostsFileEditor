#ifndef DOMAINDELEGATE_H
#define DOMAINDELEGATE_H

#include <QStyledItemDelegate>

class DomainDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DomainDelegate(QObject *parent = Q_NULLPTR);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // DOMAINDELEGATE_H
