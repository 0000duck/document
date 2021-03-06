package web

import (
	"net/http"
)

func init() {
	Instance()
}

var instance *singleRouter

type singleRouter struct {
	staticRounter map[string]int

	names map[string]http.Handler

	controller map[string]http.Handler
}

func Instance() *singleRouter {
	if instance == nil {
		instance = &singleRouter{}
		instance.staticRounter = make(map[string]int)
		instance.names = make(map[string]http.Handler)
		instance.controller = make(map[string]http.Handler)
	}
	return instance
}

func (s *singleRouter) AddStaticRounter(url string) {
	s.staticRounter[url] = 0
}
func (s *singleRouter) StaticRounter() map[string]int {
	return s.staticRounter
}
func (s *singleRouter) RegisterController(name string, h http.Handler) {
	s.names[name] = h
}

func (s *singleRouter) AddController(url, name string) {
	s.controller[url] = s.names[name]
}
func (s *singleRouter) Controller() map[string]http.Handler {
	return s.controller
}
