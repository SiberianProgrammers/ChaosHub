/// @author M. A. Serebrennikov
#include "Record.h"
#include "RecordContent.h"
#include "RecordMaster.h"
#include "Storage.h"
#include "Tools.h"
#include "sqlite3.h"
#include "TextNoteMaster.h"

#include <QDebug>

using namespace sp;

RecordMaster &RecordMaster::instance()
{
    static RecordMaster master;
    return master;
}

//------------------------------------------------------------------------------
QVector<RecordPtr> RecordMaster::get(const QString &tag, int limit, int offset)
{
    QVector<RecordPtr> result;

    sqlite3_stmt *stmt;
    defer(sqlite3_finalize(stmt));

    const char *query = "SELECT `id`, `type`, `contentId`, `date` from `Records` "
                        "ORDER BY `date` DESC";
    sqlite3_prepare_v2(StorageI.db(), query, -1, &stmt, NULL);

    for(;;) {
        int res = sqlite3_step (stmt);

        if (res == SQLITE_ROW) {
            const void* buf = sqlite3_column_blob(stmt, 0);
            int bufsize = sqlite3_column_bytes(stmt, 0);
            QUuid id;
            switch(bufsize) {
                case 38: // количество символов в человеческом представлении UUID
                    id = QUuid(QByteArray(reinterpret_cast<char *>(const_cast<void*>(buf)), bufsize));
                    break;
                case sizeof(QUuid):
                    id = QUuid::fromRfc4122(QByteArray(reinterpret_cast<char *>(const_cast<void*>(buf)), bufsize));
                    break;

                default:
                    qCritical() << "Error of loading id of record, bufsize =" << bufsize;
                    Q_ASSERT(false);
                    continue;
            }

            auto type = static_cast<Record::Type>(sqlite3_column_int(stmt, 1));
            auto contentId = sqlite3_column_int(stmt, 2);
            auto date = QDateTime::fromMSecsSinceEpoch(sqlite3_column_int64(stmt, 3));

            RecordPtr record(new Record(id,type,loadContent(type, contentId), date));
            result.append(record);
        } else if (res == SQLITE_DONE) {
            break;
        } else {
            qCritical() << "Error of record loading";
            break;
        }
    } // for(;;) {
    return result;
}

//------------------------------------------------------------------------------
void RecordMaster::append(const RecordPtr &record)
{
    Q_ASSERT(record->content());
    Q_ASSERT(record->content()->rowid() > 0);

    sqlite3_stmt *stmt;
    defer(sqlite3_finalize(stmt));

    const char *query = "INSERT INTO `Records` (`id`, `type`, `contentId`, `date`) "
                        "VALUES (?1, ?2, ?3, ?4)";
    sqlite3_prepare_v2(StorageI.db(), query, -1, &stmt, NULL);

    // TODO На момент отладки принимаются человеческое представление UUID. Потом раскоментировать
    // sqlite3_bind_blob(stmt, 1, &record->id(), sizeof(QUuid), SQLITE_STATIC);
    QByteArray buf = record->id().toByteArray();
    sqlite3_bind_blob(stmt, 1, buf.data(), buf.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, record->type());
    sqlite3_bind_int(stmt, 3, record->content()->rowid());
    sqlite3_bind_int64(stmt, 4, record->date().toMSecsSinceEpoch());

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("ERROR inserting data: %s\n", sqlite3_errmsg(StorageI.db()));
        return;
    }

    emit recordCreated(record);
}

//------------------------------------------------------------------------------
void RecordMaster::removeRaw(const RecordPtr &record)
{
}

//------------------------------------------------------------------------------
void RecordMaster::removeRaw(const QVector<RecordPtr> &records)
{

}

//------------------------------------------------------------------------------
void RecordMaster::removeRaw(const QVector<QUuid> &records)
{

}

//------------------------------------------------------------------------------
void RecordMaster::removeRaw(Record *recordRaw)
{
    if (!recordRaw) {
        Q_ASSERT(false);
        return;
    }
    auto record = recordRaw->sharedFromThis();

    if (!record->content()->remove()) {
        return;
    }

    Q_ASSERT(record->content());
    Q_ASSERT(record->content()->rowid() > 0);

    sqlite3_stmt *stmt;
    defer(sqlite3_finalize(stmt));

    const char *query = "DELETE FROM `Records` WHERE `id`=?1";
    sqlite3_prepare_v2(StorageI.db(), query, -1, &stmt, NULL);

    // TODO На момент отладки принимаются человеческое представление UUID. Потом раскоментировать
    // sqlite3_bind_blob(stmt, 1, &record->id(), sizeof(QUuid), SQLITE_STATIC);
    QByteArray buf = record->id().toByteArray();
    sqlite3_bind_blob(stmt, 1, buf.data(), buf.size(), SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        qCritical("Error of removing record: %s\n", sqlite3_errmsg(StorageI.db()));
        return;
    }

    emit recordRemoved(record);
}

//------------------------------------------------------------------------------
RecordContentPtr RecordMaster::loadContent(Record::Type type, int rowid)
{
    switch(type) {
        case Record::TextType: {
            return TextNoteMasterI.get(rowid);
        }
    }

    return  {};
}
