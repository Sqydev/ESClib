default:
	just local

local:
	cmake --preset local
	cmake --build --preset local

tester:
	cmake --preset tester
	cmake --build --preset tester

debug:
	cmake --preset debug
	cmake --build --preset debug

bleeding:
	docker compose up --build bleeding
normal:
	docker compose up --build normal
stable:
	docker compose up --build stable
musl:
	docker compose up --build musl
windows:
	docker compose up --build windows
all:
	just bleeding normal stable musl windows
