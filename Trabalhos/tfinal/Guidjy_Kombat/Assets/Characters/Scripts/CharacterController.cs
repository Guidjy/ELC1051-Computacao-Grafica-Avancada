using UnityEngine;
using UnityEngine.InputSystem;

public class CharacterController : MonoBehaviour
{
    [SerializeField] 
    private InputActionReference moveAction;

    [SerializeField]
    private float movementSpeed = 5f;

    public Vector2 Move { get; private set; }

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        moveAction.action.Enable();
    }

    // Update is called once per frame
    void Update()
    {
        Move = moveAction.action.ReadValue<Vector2>();

        Vector3 pos = transform.position;
        pos.x += Move.x * movementSpeed * Time.deltaTime;
        pos.y += Move.y * movementSpeed * Time.deltaTime;
        transform.position = pos;
    }
}
