# Reflections on the Program

## 1. What Did You Find Most Challenging with This Program?
The most challenging aspect of the program was managing the parsing of the CSV file, particularly handling quoted strings that contained embedded commas (Instructor's name field), which complicated the extraction of individual fields.

## 2. What Problems Did You Encounter and How Did You Solve Them?
- **Quoted Strings in CSV**: Handling quoted fields required implementing logic to identify and process them separately, ensuring commas inside quotes did not disrupt the parsing.

## 3. What Did You Learn from Writing This Program?
- Gained a deeper understanding of file parsing techniques, particularly for CSV files with complex structures.
- Recognized the importance of robust error handling to address unexpected formatting issues.
- Learned effective use of STL containers like `std::map` for structured and efficient data storage and retrieval.
- Realized that searching with partial keys has a linear runtime, as all entries must be checked to identify matches.
- Added an optional search by key method, enabling efficient lookups for complete keys.
