#pragma once

#include "renderable.hh"

namespace CEngine {
class render_device {
public:
	render_device() = default;
	virtual ~render_device() = default;
  virtual void initialize() = 0;
  virtual void flush()      = 0;
  virtual void terminate()  = 0;

  virtual void render(const renderable &item) const = 0;
};
} // namespace CEngine
