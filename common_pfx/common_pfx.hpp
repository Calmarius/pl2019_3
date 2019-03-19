#pragma once

#include "../libpfx/pfx.hpp"

namespace cpfx
{
void applyCommonPfx(pfx::Context &ctx);

std::shared_ptr<pfx::Command> createLambda(pfx::ArgIterator &iter);
} // namespace cpfx
