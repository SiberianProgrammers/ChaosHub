/// @author M. A. Serebrennikov
#pragma once

#include "PointersTypedef.h"
#include "Record.h"

#include <QObject>
#include <QVector>

namespace sp {

/** Быстрое обращение к синглетону. */
#define RecordMasterI RecordMaster::instance()

/*************************************************************************//**
 * @brief Синглетон производит загрузку, удаление и изменение объектов
 * класса Record.
 ****************************************************************************/
class RecordMaster: public QObject
{
    Q_OBJECT

    public:
        /** Возвращает синглетон. */
        static RecordMaster& instance();

        //--------------------------------------------------------------------
        // Get
        //--------------------------------------------------------------------
        /**
         * @brief Возвращает список записей по тегу, загруженных из хранилища.
         * @details
         */
        QVector<RecordPtr> get(const QString &tag, int limit, int offset = 0);

        //--------------------------------------------------------------------
        // Special
        //--------------------------------------------------------------------
        /** Добавляет запись в хранилище. */
        void addRecord(const RecordPtr &record);

        /** Удаляет запись из хранилища и приложения. */
        void remove(const RecordPtr &record);
        void remove(const QVector<RecordPtr> &records);
        void remove(const QVector<QUuid> &records);

    private:
        RecordContentPtr loadContent(Record::Type type, int rowid);

    signals:
        void recordRemoved(RecordPtr record) const;
        void recordCreated(RecordPtr record) const;

    private:
        RecordMaster() = default;
};

} // namespace sp