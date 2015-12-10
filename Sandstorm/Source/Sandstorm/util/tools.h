// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class SANDSTORM_API tools
{
public:
	tools();
	~tools();
};

class range {
public:
    class iterator {
        friend class range;
    public:
        long int operator *() const { return i_; }
        const iterator &operator ++() { ++i_; return *this; }
        iterator operator ++(int) { iterator copy(*this); ++i_; return copy; }
        
        bool operator ==(const iterator &other) const { return i_ == other.i_; }
        bool operator !=(const iterator &other) const { return i_ != other.i_; }
        
    protected:
        iterator(long int start) : i_ (start) { }
        
    private:
        unsigned long i_;
    };
    
    iterator begin() const { return begin_; }
    iterator end() const { return end_; }
    range(long int  begin, long int end) : begin_(begin), end_(end) {}
    range(long int end) : begin_(0), end_(end) {}
private:
    iterator begin_;
    iterator end_;
};