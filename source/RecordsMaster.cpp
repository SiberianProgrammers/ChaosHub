/// @author M. A. Serebrennikov
#include "Record.h"
#include "RecordsMaster.h"
#include "Storage.h"

using namespace sp;

RecordsMaster &RecordsMaster::instance()
{
    static RecordsMaster master;
    return master;
}

//------------------------------------------------------------------------------
QVector<RecordPtr> RecordsMaster::get(const QString &tag, int limit, int offset)
{
    QVector<RecordPtr> result;


    return result;
}

//------------------------------------------------------------------------------
void RecordsMaster::addRecord(const RecordPtr &record)
{
}

//------------------------------------------------------------------------------
void RecordsMaster::remove(const RecordPtr &record)
{
}

//------------------------------------------------------------------------------
void RecordsMaster::remove(const QVector<RecordPtr> &records)
{

}

//------------------------------------------------------------------------------
void RecordsMaster::remove(const QVector<QUuid> &records)
{

}
