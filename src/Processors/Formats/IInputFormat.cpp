#include <Processors/Formats/IInputFormat.h>
#include <IO/ReadBuffer.h>
#include <Poco/Logger.h>
#include "Common/logger_useful.h"


namespace DB
{

IInputFormat::IInputFormat(Block header, ReadBuffer * in_)
    : ISource(std::move(header)), in(in_)
{
    column_mapping = std::make_shared<ColumnMapping>();
}

IInputFormat::~IInputFormat()
{
    LOG_DEBUG(&Poco::Logger::get("IInputFormat"), "InputFormat is being desctructed. It has {} owned buffer.", owned_buffers.size());
    CurrentThread::getGroup()->memory_tracker.getParent()->logCurrentMemoryUsage();
}

void IInputFormat::resetParser()
{
    chassert(in);
    in->ignoreAll();
    // those are protected attributes from ISource (I didn't want to propagate resetParser up there)
    finished = false;
    got_exception = false;

    getPort().getInputPort().reopen();
}

void IInputFormat::setReadBuffer(ReadBuffer & in_)
{
    chassert(in); // not supported by random-access formats
    in = &in_;
}

}
