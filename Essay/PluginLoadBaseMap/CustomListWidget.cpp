#include "CustomListWidget.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#pragma execution_character_set("utf-8")

CustomListWidget::CustomListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    loadPlugins();
}

#if 0
void CustomListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mDragStartPos = event->pos();
}

void CustomListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - mDragStartPos).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag *    drag = new QDrag(this);
    QMimeData *mime = new QMimeData;

    mime->setText(currentItem()->text());
    drag->setMimeData(mime);

    qDebug() << "Mouse Move Event";
    if (drag->exec() == Qt::MoveAction)
    {
        qDebug() << "Return Move Action";
    }
}

#endif
void CustomListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
    //        event->accept();
}

void CustomListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    QList<QUrl> urls = event->mimeData()->urls();
    for (int i = 0; i < urls.size(); ++i)
    {
        QString         path = urls.at(i).toLocalFile();
        QMimeDatabase   mimeDatabase;
        const QFileInfo fi(path);
        const QMimeType mimeType = mimeDatabase.mimeTypeForFile(fi);
        const QString   suffix   = mimeType.preferredSuffix();

        BaseMapInterface *loader = mLoaders.value(suffix, nullptr);
        if (loader)
        {
            if (loader->loadFromFile(path))
            {
                addItem(fi.fileName());
                QPointF pos = loader->pos();
                emit    addImage(fi.fileName(), loader->baseMap(), pos);
                qDebug() << "loader pos" << pos;
            }
            else
            {
                QMessageBox::critical(this, "加载文件失败", "加载文件失败：" + path);
            }
        }
        else
        {
            QMessageBox::critical(this, "加载文件失败", "不支持此类型的文件");
        }
    }
    event->accept();
    slotUpdateImageLevel();
}

void CustomListWidget::slotUpdateImageLevel()
{
    int _count = count();
    for (int i = 0; i < _count; ++i)
    {
        QListWidgetItem *_item = item(i);
        if (_item)
        {
            emit changeImageLevel(_item->text(), _count - i);
        }
    }
}

void CustomListWidget::loadPlugins()
{
    QDir pluginsDir(R"(D:\Qt\build-Test-Desktop_Qt_5_9_5_MSVC2015_64bit-Release\customBaseMap\release)");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        // 对加载的插件做处理
        QPluginLoader     loader(pluginsDir.absoluteFilePath(fileName));
        BaseMapInterface *iBaseMap = qobject_cast<BaseMapInterface *>(loader.instance());
        if (iBaseMap)
        {
            QString filter = iBaseMap->filter();
            qDebug() << "filter:" << filter;
            qDebug() << "suffix:" << iBaseMap->suffix();
            qDebug() << "name:" << iBaseMap->name();
            qDebug() << "description:" << iBaseMap->description();

            if (!mFilters.contains(filter))
                mFilters.append(filter);

            mLoaders.insert(iBaseMap->suffix(), iBaseMap);
        }
    }
}

#if 0
void CustomListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(mMimeType))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
#endif
