// Generated by gencpp from file distance/BoundingBox.msg
// DO NOT EDIT!


#ifndef DISTANCE_MESSAGE_BOUNDINGBOX_H
#define DISTANCE_MESSAGE_BOUNDINGBOX_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace distance
{
template <class ContainerAllocator>
struct BoundingBox_
{
  typedef BoundingBox_<ContainerAllocator> Type;

  BoundingBox_()
    : probability(0.0)
    , xmin(0)
    , ymin(0)
    , xmax(0)
    , ymax(0)
    , num(0)
    , Class()  {
    }
  BoundingBox_(const ContainerAllocator& _alloc)
    : probability(0.0)
    , xmin(0)
    , ymin(0)
    , xmax(0)
    , ymax(0)
    , num(0)
    , Class(_alloc)  {
  (void)_alloc;
    }



   typedef double _probability_type;
  _probability_type probability;

   typedef int64_t _xmin_type;
  _xmin_type xmin;

   typedef int64_t _ymin_type;
  _ymin_type ymin;

   typedef int64_t _xmax_type;
  _xmax_type xmax;

   typedef int64_t _ymax_type;
  _ymax_type ymax;

   typedef int16_t _num_type;
  _num_type num;

   typedef std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> _Class_type;
  _Class_type Class;





  typedef boost::shared_ptr< ::distance::BoundingBox_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::distance::BoundingBox_<ContainerAllocator> const> ConstPtr;

}; // struct BoundingBox_

typedef ::distance::BoundingBox_<std::allocator<void> > BoundingBox;

typedef boost::shared_ptr< ::distance::BoundingBox > BoundingBoxPtr;
typedef boost::shared_ptr< ::distance::BoundingBox const> BoundingBoxConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::distance::BoundingBox_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::distance::BoundingBox_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::distance::BoundingBox_<ContainerAllocator1> & lhs, const ::distance::BoundingBox_<ContainerAllocator2> & rhs)
{
  return lhs.probability == rhs.probability &&
    lhs.xmin == rhs.xmin &&
    lhs.ymin == rhs.ymin &&
    lhs.xmax == rhs.xmax &&
    lhs.ymax == rhs.ymax &&
    lhs.num == rhs.num &&
    lhs.Class == rhs.Class;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::distance::BoundingBox_<ContainerAllocator1> & lhs, const ::distance::BoundingBox_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace distance

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::distance::BoundingBox_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::distance::BoundingBox_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::distance::BoundingBox_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::distance::BoundingBox_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::distance::BoundingBox_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::distance::BoundingBox_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::distance::BoundingBox_<ContainerAllocator> >
{
  static const char* value()
  {
    return "73bbf1ba82d340a009fab2e8489093f5";
  }

  static const char* value(const ::distance::BoundingBox_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x73bbf1ba82d340a0ULL;
  static const uint64_t static_value2 = 0x09fab2e8489093f5ULL;
};

template<class ContainerAllocator>
struct DataType< ::distance::BoundingBox_<ContainerAllocator> >
{
  static const char* value()
  {
    return "distance/BoundingBox";
  }

  static const char* value(const ::distance::BoundingBox_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::distance::BoundingBox_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 probability\n"
"int64 xmin\n"
"int64 ymin\n"
"int64 xmax\n"
"int64 ymax\n"
"int16 num\n"
"string Class\n"
;
  }

  static const char* value(const ::distance::BoundingBox_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::distance::BoundingBox_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.probability);
      stream.next(m.xmin);
      stream.next(m.ymin);
      stream.next(m.xmax);
      stream.next(m.ymax);
      stream.next(m.num);
      stream.next(m.Class);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct BoundingBox_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::distance::BoundingBox_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::distance::BoundingBox_<ContainerAllocator>& v)
  {
    s << indent << "probability: ";
    Printer<double>::stream(s, indent + "  ", v.probability);
    s << indent << "xmin: ";
    Printer<int64_t>::stream(s, indent + "  ", v.xmin);
    s << indent << "ymin: ";
    Printer<int64_t>::stream(s, indent + "  ", v.ymin);
    s << indent << "xmax: ";
    Printer<int64_t>::stream(s, indent + "  ", v.xmax);
    s << indent << "ymax: ";
    Printer<int64_t>::stream(s, indent + "  ", v.ymax);
    s << indent << "num: ";
    Printer<int16_t>::stream(s, indent + "  ", v.num);
    s << indent << "Class: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>::stream(s, indent + "  ", v.Class);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DISTANCE_MESSAGE_BOUNDINGBOX_H
