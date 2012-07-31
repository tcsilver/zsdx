local item = ...

function item:on_use()

  if self:get_amount() == 0 then
    sol.audio.play_sound("wrong")
  else
    self:remove_amount(1)
    self:get_game():add_life(4 * 3)
  end
  self:set_finished()
end

