open Printf

module Str_map = Map.Make(String)

module Dir = struct
  type file_t = { name: string; size: int }
  type t = { name: string; subdirs: t Str_map.t; files: int Str_map.t }

  let with_root name =
    { name; subdirs = Str_map.empty; files = Str_map.empty }

  let find_subdir current dirname =
    Str_map.find_opt dirname current.subdirs

  let create_subdir directory name =
    { directory with subdirs = Str_map.add name (with_root name) directory.subdirs }

  let create_file directory name size =
    { directory with files = Str_map.add name size directory.files }

  let put_subdir directory subdir =
    { directory with subdirs = Str_map.add subdir.name subdir directory.subdirs }

  let total_sizes directory =
    let rec total_sizes' { name; subdirs; files } path results =
      let current_filesize = Str_map.fold (fun _ size result -> result + size) files 0 in
      let (dir_totals, subdir_totals) = Str_map.fold (fun name subdir (dir_totals, subdir_totals) ->
        let new_path = if path = "/" then path ^ name else path ^ "/" ^ name in
        let results = total_sizes' subdir new_path dir_totals in
        let subdir_size = Str_map.find new_path results in
        (results, subdir_totals + subdir_size)
      ) subdirs (results, 0)
      in
      Str_map.add path (current_filesize + subdir_totals) dir_totals
    in
    total_sizes' directory directory.name (Str_map.empty)
end

module Dir_zipper = struct
  exception InvalidPosition of string
  type t = { current: Dir.t; parents: t list }

  let at_root { parents; _ }=
    (List.length parents) = 0

  let move_up { current; parents } =
    match parents with
    | { current = parent; parents } :: _ -> { current = Dir.put_subdir parent current; parents }
    | _ -> raise (InvalidPosition (sprintf "already at top of dir_tree"))

  let move_into zipper dirname =
    match Dir.find_subdir zipper.current dirname with
    | Some subdir -> { current = subdir; parents = zipper :: zipper.parents }
    | _ -> raise (InvalidPosition (sprintf "subdir %s does not exist" dirname))

  let put_directory zipper name =
    { zipper with current = Dir.create_subdir zipper.current name }

  let put_file zipper filename size =
    { zipper with current = Dir.create_file zipper.current filename size }

  let of_dir dir =
    { current = dir; parents = [] }

  let rec to_dir zipper =
    if at_root zipper then zipper.current else to_dir (move_up zipper)
end

exception BadInput of string
exception BadFileSystem of string

type command_t =
  | ChangeDir of string
  | ListDir

type input_t =
  | Command of command_t
  | Directory of string
  | File of (string * int)

let read_command_file filename =
  let input = open_in filename in
  let rec read_file' lines =
    try
      let line = input_line input in
      read_file' (line :: lines)
    with End_of_file ->
      let () = close_in input in
      List.rev lines
  in
  read_file' []

let parse_input input_lines =
  let parse_command input =
    match input with
    | "cd" :: [target] -> Command (ChangeDir target)
    | "ls" :: [] -> Command ListDir
    | _ ->
        let full_command = String.concat " " input in
        raise (BadInput (sprintf "invalid command: %s" full_command))
  in
  List.map (fun line ->
    let tokens = String.split_on_char ' ' line in
    match tokens with
    | "$" :: rest -> parse_command rest
    | "dir" :: [name] -> Directory name
    | size :: [name] -> File (name, (int_of_string size))
    | _ -> raise (BadInput (sprintf "unrecognized line: %s" line))
  ) input_lines

let build_dir commands =
  let rec build_dir' zipper = function
    | Command ListDir -> zipper
    | Command (ChangeDir path) when path = ".." -> Dir_zipper.move_up zipper
    | Command (ChangeDir path) -> Dir_zipper.move_into zipper path
    | Directory name -> Dir_zipper.put_directory zipper name
    | File (name, size) -> Dir_zipper.put_file zipper name size
  in
  match commands with
  | Command (ChangeDir path) :: input ->
      let zipper = Dir.with_root path |> Dir_zipper.of_dir in
      List.fold_left build_dir' zipper input |> Dir_zipper.to_dir
  | _ -> raise (BadFileSystem "expected command to change to the root path")

let calculate_part1 ~maximum directory_sizes =
  let total = Str_map.fold (fun _ size total_size ->
    if size <= maximum then total_size + size else total_size
  ) directory_sizes 0
  in
  print_endline (sprintf "total is %d" total)

let calculate_part2 ~capacity ~required directory_sizes =
  let root_size = Str_map.find "/" directory_sizes in
  let space_needed = required - (capacity - root_size) in
  let smallest = Str_map.fold (fun _ size smallest ->
    if size >= space_needed && size < smallest then size else smallest
  ) directory_sizes capacity
  in
  print_endline (sprintf "smallest is %d" smallest)

let main () =
  let directory_sizes = read_command_file "input" |> parse_input |> build_dir |> Dir.total_sizes in
  let () = calculate_part1 ~maximum:100000 directory_sizes in
  calculate_part2 ~capacity:70000000 ~required:30000000 directory_sizes

let () = main ()
