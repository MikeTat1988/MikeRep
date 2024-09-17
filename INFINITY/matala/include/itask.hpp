/******************************************************************************
  Project:      HOME_ASSIGNMENT
  Description:  ITASK interface
  Author:       Michael
  Date:         05/14/23
  Version:      1.0
 ******************************************************************************/

#ifndef HAM_ITASK_HPP
#define HAM_ITASK_HPP

namespace ham
{

enum class Priority
{
    LOW = 0,
    MEDIUM,
    HIGH,
    // add new here
    ADMIN
};

class ITask
{
public:
    explicit ITask(Priority priority_ = Priority::LOW) : m_priority(priority_) {}
    virtual ~ITask() = default;

    ITask(const ITask&) = default;
    ITask& operator=(const ITask&) = delete;

    bool operator<(const ITask& other_) const;

    virtual void Execute() = 0;
private:
    friend class ThreadPool;
    const Priority m_priority;

};//class ITask


inline bool ITask::operator<(const ITask& other_) const
{
    return m_priority < other_.m_priority;
}

}//namespace ham


#endif // HAM_ITASK_HPP


