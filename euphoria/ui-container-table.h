// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_CONTAINER_TABLE_H_
#define EUPHORIA_UI_CONTAINER_TABLE_H_

#include <vector>
#include <memory>
#include <string>

#include "euphoria/ui-value.h"
#include "euphoria/ui-container.h"

namespace euphoria {

class TextureCache;
class ShaderCache;
class Settings;
class Camera;

namespace ui {

class Cell {
 public:
  Cell(std::shared_ptr<Container> widget, const Vec2i& position,
       const Vec2i& size);
  Vec2i tile_position() const;
  Vec2i tile_size() const;

  void SetPositionAndSize(Vec2 position, Vec2 size) const;
  void Draw(const Camera& camera) const;

 private:
  std::shared_ptr<Container> container_;
  Vec2i position_;
  Vec2i size_;
};

class Size {
 public:
  Size();
  Value size;
};

class TableContainer : public Container {
 public:
  TableContainer();
  ~TableContainer();

  const std::vector<Size>& rows() const;
  void set_rows(const std::vector<Size>& rows);

  const std::vector<Size>& columns() const;
  void set_columns(const std::vector<Size>& columns);

  void AddColoumn(Size column);
  void AddRow(Size row);
  void AddCell(Cell cell);

  void Layout() override;

  void Draw(const Camera& camera) override;

 private:
  std::vector<Size> rows_;
  std::vector<Size> columns_;
  std::vector<Cell> cells_;
};

void LoadTable(TableContainer* table, const std::string& filename,
               TextureCache* tcache, ShaderCache* scache,
               const Settings& settings);

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_CONTAINER_TABLE_H_
