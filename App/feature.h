#pragma once

class ReadWriteLock;

class Feature
{
public:
    Feature(ReadWriteLock &lock);

    bool doSomething();

    int getSomething() const;

private:
    ReadWriteLock &m_rwLock;
};
