#include "taag_client.h"

Chat_widget::Chat_widget(class Game_engine* p_engine, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_console_selected = false;
	_starting_line = 0;
	_nb_line = 6;
	_engine = p_engine;
	_box = jgl::w_box_component(this);

	_entry_box = jgl::w_box_component(this);
	_entry = jgl::w_text_entry_component("", this);
	_entry.set_back(jgl::Color(255, 0, 0));
	_entry.set_front(jgl::Color(255, 0, 0));
	_next_input = 0;
	_input_delay = 200;
}

void Chat_widget::reset_line()
{
	for (size_t i = 0; i < _lines.size(); i++)
	{
		for (size_t j = 0; j < _lines[i].text_images.size(); j++)
		{
			delete _lines[i].text_images[j];
			_lines[i].text_images[j] = nullptr;
		}
		_lines[i].text_images.clear();
	}
}
void Chat_widget::generate(size_t index)
{
	if (_lines.size() <= index)
		return;

	Chat_line& line = _lines[index];
	static jgl::Array<jgl::String> tab;
	jgl::Image* tmp_image = jgl::get_string_image("Mpdsquhilaze", _text_size, 0, jgl::text_color::black, jgl::text_style::normal);
	tab.clear();

	if (_lines[index].line.size() != 0)
	{
		jgl::strsplit(tab, line.line, " ", false);
		size_t i = 0;
		jgl::String to_draw = "";
		size_t line_size = static_cast<size_t>(_viewport->area().y / _nb_line);
		float ratio = _text_size / tmp_image->size().y;
		line_size *= ratio;
		while (i < tab.size())
		{
			if (tab[i].size() > 25)
			{
				jgl::String first = tab[i].substr(0, 25);
				jgl::String second = tab[i].substr(25, tab[i].size());

				tab.erase(i);
				tab.insert(i, second);
				tab.insert(i, first);
			}

			if (jgl::calc_text_len(to_draw + " " + tab[i], line_size) < _viewport->area().x)
			{
				to_draw += " " + tab[i];
			}
			else
			{
				line.text_images.push_back(jgl::get_string_image(to_draw, _text_size, 0, jgl::text_color::black, jgl::text_style::normal));
				to_draw = "    " + tab[i];
			}
			i++;
		}
		if (to_draw != "")
			line.text_images.push_back(jgl::get_string_image(to_draw, _text_size, 0, jgl::text_color::black, jgl::text_style::normal));
	}
	else
	{
		line.text_images.push_back(nullptr);
	}	
}

void Chat_widget::clear_line()
{
	for (size_t i = 0; i < _lines.size(); i++)
	{
		for (size_t j = 0; j < _lines[i].text_images.size(); j++)
		{
			delete _lines[i].text_images[j];
			_lines[i].text_images[j] = nullptr;
		}
		_lines[i].text_images.clear();
	}
	_lines.clear();
}

void Chat_widget::add_line(jgl::String new_line)
{
	_lines.push_front(Chat_line(new_line));
}

bool Chat_widget::handle_keyboard()
{
	if (_entry.selected() == true)
	{
		if (jgl::get_key(jgl::key::return_key) == jgl::key_state::release || jgl::get_key(jgl::key::key_pad_return_key) == jgl::key_state::release)
		{
			_engine->send_chat_message(_entry.text());
			_entry.set_text("");
			return (true);
		}
		static Uint32 ticks = 0;
		if (g_keyboard->get_key(jgl::key::left_arrow) == jgl::key_state::down && ticks < SDL_GetTicks() && (ticks = SDL_GetTicks() + 100))
			_entry.move_cursor(-1);
		else if (g_keyboard->get_key(jgl::key::right_arrow) == jgl::key_state::down && ticks < SDL_GetTicks() && (ticks = SDL_GetTicks() + 100))
			_entry.move_cursor(1);
		else if (g_keyboard->get_key(jgl::key::backspace) == jgl::key_state::down && g_application->time() >= _next_input)
		{
			_entry.remove_text();
			_next_input = g_application->time() + _input_delay / 2;
		}
		else if (g_keyboard->get_key(jgl::key::delete_key) == jgl::key_state::down && g_application->time() >= _next_input)
		{
			_entry.supp_text();
			_next_input = g_application->time() + _input_delay / 2;
		}

		if (g_application->event()->type == SDL_TEXTINPUT)
		{
			static jgl::Glyph last_char = '\0';
			jgl::Glyph text = g_application->event()->text.text;
			if (text != last_char || g_application->time() >= _next_input)
			{
				last_char = text;
				_entry.add_text(text);
				_next_input = g_application->time() + _input_delay;
			}
		}
	}
	return (false);
}

bool Chat_widget::handle_mouse()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		if (_entry.is_pointed(g_mouse->pos) == true)
		{
			_entry.set_selected(true);
			return (true);
		}
		else
		{
			_entry.set_selected(false);
			return (true);
		}
	}
	if (_lines.size() != 0 && g_mouse->wheel != 0 && is_pointed(g_mouse->pos) == true)
	{
		if (g_mouse->wheel > 0)
		{
			_starting_line += g_mouse->wheel;
		}
		else if (g_mouse->wheel < 0)
		{
			_starting_line += g_mouse->wheel;
		}
		size_t tmp = (_lines.size() > _nb_line ? _lines.size() - _nb_line : 0);
		if (_starting_line < 0)
			_starting_line = 0;
		else if (_starting_line > tmp)
			_starting_line = tmp;
	}
	
	return (false);
}

void Chat_widget::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
	float tmp = (_viewport->area().y - _entry_box.area().y) / 5;

	_entry_box.set_area(jgl::Vector2(p_area.x, tmp) - _box.border() * 4);
	_entry_box.set_anchor(p_anchor + jgl::Vector2(static_cast<float>(_entry_box.border() * 2), p_area.y - tmp + _entry_box.border() * 2));

	_entry.set_area(_entry_box.area() - _entry_box.border() * 2);
	_entry.set_anchor(_entry_box.anchor() + _entry_box.border());

	_entry.calc_text_size_height(_entry.area());
	_entry.calc_text_to_draw();
	reset_line();
	_text_size = static_cast<size_t>((_viewport->area().y - _entry_box.area().y - 5) / _nb_line);
	_start_pos = jgl::Vector2(5.0f, p_area.y - _entry_box.area().y - _text_size - 10);
}

void Chat_widget::render()
{
	_box.render(_viewport);
	_entry_box.render(_viewport);
	_entry.render(_viewport);

	int nb_line = 0;
	jgl::Vector2 pos = _start_pos;
	if (_lines.size() != 0)
	{
		for (size_t i = _starting_line; i < _lines.size() && nb_line < _nb_line; i++)
		{
			if (_lines[i].line != "" && _lines[i].text_images.size() == 0)
				generate(i);
			for (int j = _lines[i].text_images.size() - 1; j >= 0 && nb_line < _nb_line; j--)
			{
				float ratio = _text_size / _lines[i].text_images[j]->size().y;
				if (_lines[i].text_images[j] != nullptr)
					_lines[i].text_images[j]->draw(pos, _lines[i].text_images[j]->size() * ratio + 2, 1.0f, _viewport);
				pos.y -= _text_size;
				nb_line++;
			}
		}
	}
}
