using Characters.States;
using System.Linq;
using UnityEngine;
using UnityEngine.InputSystem;

namespace Characters.Movement
{
    public class CharacterController : MonoBehaviour
    {
        [SerializeField] private float movementSpeed = 2.7f;
        [SerializeField] private float jumpSpeed = 5f;

        [SerializeField] private InputActionReference moveAction;

        [SerializeField] private LayerMask groundLayer;

        private Collider characterCollider;
        private Rigidbody rigidBody;
        private SpriteRenderer spriteRenderer;

        private Vector2 velocity;
        private Vector2 movementDirection;
        private MovementStates movementState;

        private bool stickIsUp;
        private bool stickWasUp;

        private bool jumpRequested;
        private int remainingJumps;

        private Transform opponentTransform;
        private float distanceFromOpponent;
        private bool shouldFlipHorizontaly;  // Used to control when the character should flip to face the opponent

        private void Awake()
        {
            characterCollider = GetComponent<Collider>();
            rigidBody = GetComponent<Rigidbody>();
            spriteRenderer = GetComponent<SpriteRenderer>();
        }

        void Start()
        {
            opponentTransform = FindObjectsByType<CharacterController>().Where(opponent => opponent.transform != transform).First().transform;
            movementState = MovementStates.STANDING;
            stickIsUp = false;
            stickWasUp = false;
        }

        void OnEnable()
        {
            moveAction.action.Enable();
        }

        void OnDisable()
        {
            moveAction.action.Disable();
        }


        void Update()
        {
            // Updates movement direction
            movementDirection = moveAction.action.ReadValue<Vector2>();
            movementDirection.x = Mathf.Round(movementDirection.x);
            movementDirection.y = Mathf.Round(movementDirection.y);

            // Checks if a jump was requested
            stickIsUp = movementDirection.y > 0;
            if (stickIsUp && !stickWasUp) jumpRequested = true;
            stickWasUp = stickIsUp;

            FaceOpponent();
        }

        private void FixedUpdate()
        {
            velocity = rigidBody.linearVelocity;

            if (IsGrounded())
            {
                SetGrounded();

                // Moves horizontally
                velocity.x = movementDirection.x * movementSpeed;
            }

            if (jumpRequested && remainingJumps > 0)
            {
                Jump();
            }

            // Updates position
            rigidBody.linearVelocity = velocity;
            float updatedDistanceFromOpponent = transform.position.x - opponentTransform.position.x;
            if (updatedDistanceFromOpponent * distanceFromOpponent < 0) shouldFlipHorizontaly = true;
            distanceFromOpponent = updatedDistanceFromOpponent;
        }

        private bool IsGrounded()
        {
            // Distance checked for colliders below the character
            const float groundCheckDistance = 0.05f;

            Bounds characterBounds = characterCollider.bounds;
            // Gets a point just below the character's feet
            Vector3 origin = new Vector3(characterBounds.center.x, characterBounds.min.y + 0.01f, characterBounds.center.z);
            return Physics.Raycast(origin, Vector3.down, groundCheckDistance + 0.01f, groundLayer);
        }

        private void Jump()
        {
            velocity.x = movementDirection.x * movementSpeed;  // necessairy for being able to change directions when jumping
            velocity.y = jumpSpeed;

            movementState = MovementStates.JUMPING;
            jumpRequested = false;
            remainingJumps--;
        }

        private void SetGrounded()
        {
            movementState = MovementStates.STANDING;
            remainingJumps = 2;
        }

        private void FaceOpponent()
        {
            if (!shouldFlipHorizontaly)
            {
                return;
            }

            spriteRenderer.flipX = !spriteRenderer.flipX;

            shouldFlipHorizontaly = false;
        }
    }
}
