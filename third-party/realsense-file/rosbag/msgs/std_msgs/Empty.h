// Generated by gencpp from file std_msgs/Empty.msg
// DO NOT EDIT!


#ifndef STD_MSGS_MESSAGE_EMPTY_H
#define STD_MSGS_MESSAGE_EMPTY_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace std_msgs
{
template <class ContainerAllocator>
struct Empty_
{
  typedef Empty_<ContainerAllocator> Type;

  Empty_()
    {
    }
  Empty_(const ContainerAllocator& _alloc)
    {
  (void)_alloc;
    }






  typedef std::shared_ptr< ::std_msgs::Empty_<ContainerAllocator> > Ptr;
  typedef std::shared_ptr< ::std_msgs::Empty_<ContainerAllocator> const> ConstPtr;

}; // struct Empty_

typedef ::std_msgs::Empty_<std::allocator<void> > Empty;

typedef std::shared_ptr< ::std_msgs::Empty > EmptyPtr;
typedef std::shared_ptr< ::std_msgs::Empty const> EmptyConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::std_msgs::Empty_<ContainerAllocator> & v)
{
rs2rosinternal::message_operations::Printer< ::std_msgs::Empty_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace std_msgs

namespace rs2rosinternal
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/tmp/binarydeb/ros-kinetic-std-msgs-0.5.10/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::std_msgs::Empty_<ContainerAllocator> >
  : std::true_type
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::std_msgs::Empty_<ContainerAllocator> const>
  : std::true_type
  { };

template <class ContainerAllocator>
struct IsMessage< ::std_msgs::Empty_<ContainerAllocator> >
  : std::true_type
  { };

template <class ContainerAllocator>
struct IsMessage< ::std_msgs::Empty_<ContainerAllocator> const>
  : std::true_type
  { };

template <class ContainerAllocator>
struct HasHeader< ::std_msgs::Empty_<ContainerAllocator> >
  : std::false_type
  { };

template <class ContainerAllocator>
struct HasHeader< ::std_msgs::Empty_<ContainerAllocator> const>
  : std::false_type
  { };


template<class ContainerAllocator>
struct MD5Sum< ::std_msgs::Empty_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const ::std_msgs::Empty_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::std_msgs::Empty_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/Empty";
  }

  static const char* value(const ::std_msgs::Empty_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::std_msgs::Empty_<ContainerAllocator> >
{
  static const char* value()
  {
    return "\n\
";
  }

  static const char* value(const ::std_msgs::Empty_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace rs2rosinternal

namespace rs2rosinternal
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::std_msgs::Empty_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream&, T)
    {}

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Empty_

} // namespace serialization
} // namespace rs2rosinternal

namespace rs2rosinternal
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::std_msgs::Empty_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream&, const std::string&, const ::std_msgs::Empty_<ContainerAllocator>&)
  {}
};

} // namespace message_operations
} // namespace rs2rosinternal

#endif // STD_MSGS_MESSAGE_EMPTY_H
