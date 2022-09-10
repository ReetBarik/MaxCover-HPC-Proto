

#include <cstddef>
#include <memory>


template <typename BaseTy>
class Bitmask {
 public:
  Bitmask() : size_(0), data_size_(0), data_(nullptr) {}
  Bitmask(const Bitmask &O)
      : size_(O.size_),
        data_size_(O.data_size_),
        data_(new BaseTy[O.data_size_]) {
    std::memcpy(data_.get(), O.data_.get(), data_size_ * sizeof(BaseTy));
  }

  Bitmask(Bitmask &&) = default;

  explicit Bitmask(size_t num_bits)
      : size_(num_bits),
        data_size_((size_ / (8 * sizeof(BaseTy)) + 1)),
        data_(new BaseTy[(size_ / (8 * sizeof(BaseTy)) + 1)]) {
    std::memset(data_.get(), 0, data_size_ * sizeof(BaseTy));
  }

  Bitmask &operator=(const Bitmask &O) {
    size_ = O.size_;
    data_size_ = O.data_size_;
    data_ = std::unique_ptr<BaseTy[]>(new BaseTy[data_size_]);
    std::memcpy(data_.get(), O.data_.get(), data_size_ * sizeof(BaseTy));
    return *this;
  }
  Bitmask &operator=(Bitmask &&) = default;

  void set(size_t i) {
    BaseTy m = 1 << (i % (8 * sizeof(BaseTy)));
    data_[i / (8 * sizeof(BaseTy))] |= m;
  }
  bool get(size_t i) const {
    BaseTy m = 1 << (i % (8 * sizeof(BaseTy)));
    return data_[i / (8 * sizeof(BaseTy))] & m;
  }

  size_t popcount() const {
    size_t count = 0;
    for (size_t i = 0; i < data_size_; ++i) {
      count += __builtin_popcount(data_[i]);
    }
    return count;
  }

  BaseTy *data() const { return data_.get(); }
  size_t bytes() const { return data_size_ * sizeof(BaseTy); }
  size_t size() const { return size_; }

 private:
  size_t size_;
  size_t data_size_;
  std::unique_ptr<BaseTy[]> data_;
};
